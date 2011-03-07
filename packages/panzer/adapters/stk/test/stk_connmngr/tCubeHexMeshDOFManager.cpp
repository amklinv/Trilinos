#include <Teuchos_ConfigDefs.hpp>
#include <Teuchos_UnitTestHarness.hpp>
#include "Teuchos_DefaultComm.hpp"
#include "Teuchos_GlobalMPISession.hpp"
#include "Teuchos_ParameterList.hpp"

#include "Panzer_STK_Version.hpp"
#include "Panzer_STK_config.hpp"
#include "Panzer_IntrepidFieldPattern.hpp"
#include "Panzer_GeometricAggFieldPattern.hpp"
#include "Panzer_DOFManager.hpp"
#include "Panzer_STK_CubeHexMeshFactory.hpp"
#include "Panzer_STKConnManager.hpp"

#include "Intrepid_HGRAD_HEX_C1_FEM.hpp"
#include "Intrepid_HGRAD_HEX_C2_FEM.hpp"

#ifdef HAVE_MPI
   #include "Epetra_MpiComm.h"
#else
   #include "Epetra_SerialComm.h"
#endif

typedef Intrepid::FieldContainer<double> FieldContainer;

using Teuchos::RCP;
using Teuchos::rcp;
using Teuchos::rcpFromRef;

namespace panzer_stk {

Teuchos::RCP<panzer_stk::STK_Interface> buildQuadMesh(stk::ParallelMachine comm,int xelmts,int yelmts,int zelmts,
                                                                                    int xblocks,int yblocks,int zblocks)
{
   Teuchos::ParameterList pl;
   pl.set<int>("X Elements",xelmts);
   pl.set<int>("Y Elements",yelmts);
   pl.set<int>("Z Elements",zelmts);
   pl.set<int>("X Blocks",xblocks);
   pl.set<int>("Y Blocks",yblocks);
   pl.set<int>("Z Blocks",zblocks);

   panzer_stk::CubeHexMeshFactory meshFact;
   meshFact.setParameterList(Teuchos::rcpFromRef(pl));
   
   Teuchos::RCP<panzer_stk::STK_Interface> mesh = meshFact.buildMesh(comm);
   mesh->writeToExodus("whatish.exo");
   return mesh;
}

template <typename IntrepidType>
RCP<const panzer::FieldPattern> buildFieldPattern()
{
   // build a geometric pattern from a single basis
   RCP<Intrepid::Basis<double,FieldContainer> > basis = rcp(new IntrepidType);
   RCP<const panzer::FieldPattern> pattern = rcp(new panzer::IntrepidFieldPattern(basis));
   return pattern;
}

// quad tests
TEUCHOS_UNIT_TEST(tCubeHexMeshDOFManager, buildTest_hex)
{
   // build global (or serial communicator)
   #ifdef HAVE_MPI
      stk::ParallelMachine Comm = MPI_COMM_WORLD;
   #else
      stk::ParallelMachine Comm = WHAT_TO_DO_COMM;
   #endif

   int numProcs = stk::parallel_machine_size(Comm);
   int myRank = stk::parallel_machine_rank(Comm);

   TEUCHOS_ASSERT(numProcs<=2);

   // build a geometric pattern from a single basis
   RCP<const panzer::FieldPattern> patternC1 
         = buildFieldPattern<Intrepid::Basis_HGRAD_HEX_C1_FEM<double,FieldContainer> >();

   Teuchos::RCP<panzer_stk::STK_Interface> mesh = buildQuadMesh(Comm,2,2,2,1,1,1);
   RCP<panzer::ConnManager<int,int> > connManager 
         = Teuchos::rcp(new panzer_stk::STKConnManager(mesh));
   RCP<panzer::DOFManager<int,int> > dofManager = rcp(new panzer::DOFManager<int,int>());

   TEST_EQUALITY(dofManager->getConnManager(),Teuchos::null);

   dofManager->setConnManager(connManager,MPI_COMM_WORLD);
   TEST_EQUALITY(dofManager->getConnManager(),connManager);

   dofManager->addField("ux",patternC1);
   dofManager->addField("uy",patternC1);
   dofManager->addField("p",patternC1);

   std::vector<std::string> fieldOrder;
   fieldOrder.push_back("ux");
   fieldOrder.push_back("uy");
   fieldOrder.push_back("p");
   dofManager->setFieldOrder(fieldOrder);
   dofManager->printFieldInformation(out);
   dofManager->buildGlobalUnknowns();

   if(numProcs==1) {
      std::vector<int> gids_v;
      int * gids = 0;

      // element 0
      dofManager->getElementGIDs(0,gids_v);

      gids = &gids_v[0];
      TEST_EQUALITY(gids_v.size(),24);
      TEST_EQUALITY(gids[0],0);  TEST_EQUALITY(gids[1],1);   TEST_EQUALITY(gids[2],2);
      TEST_EQUALITY(gids[3],3);  TEST_EQUALITY(gids[4],4);   TEST_EQUALITY(gids[5],5);
      TEST_EQUALITY(gids[6],12); TEST_EQUALITY(gids[7],13);  TEST_EQUALITY(gids[8],14);
      TEST_EQUALITY(gids[9], 9); TEST_EQUALITY(gids[10],10); TEST_EQUALITY(gids[11],11);

      gids = &gids_v[12];
      TEST_EQUALITY(gids[0],27); TEST_EQUALITY(gids[1],28);  TEST_EQUALITY(gids[2],29);
      TEST_EQUALITY(gids[3],30); TEST_EQUALITY(gids[4],31);  TEST_EQUALITY(gids[5],32);
      TEST_EQUALITY(gids[6],39); TEST_EQUALITY(gids[7],40);  TEST_EQUALITY(gids[8],41);
      TEST_EQUALITY(gids[9],36); TEST_EQUALITY(gids[10],37); TEST_EQUALITY(gids[11],38);
   
      // element 6
      dofManager->getElementGIDs(mesh->elementLocalId(5),gids_v);

      gids = &gids_v[0];
      TEST_EQUALITY(gids_v.size(),24);
      TEST_EQUALITY(gids[0],27); TEST_EQUALITY(gids[1],28);  TEST_EQUALITY(gids[2],29);
      TEST_EQUALITY(gids[3],30); TEST_EQUALITY(gids[4],31);  TEST_EQUALITY(gids[5],32);
      TEST_EQUALITY(gids[6],39); TEST_EQUALITY(gids[7],40);  TEST_EQUALITY(gids[8],41);
      TEST_EQUALITY(gids[9],36); TEST_EQUALITY(gids[10],37); TEST_EQUALITY(gids[11],38);

      gids = &gids_v[12];
      TEST_EQUALITY(gids[0],54); TEST_EQUALITY(gids[1],55);  TEST_EQUALITY(gids[2],56);
      TEST_EQUALITY(gids[3],57); TEST_EQUALITY(gids[4],58);  TEST_EQUALITY(gids[5],59);
      TEST_EQUALITY(gids[6],66); TEST_EQUALITY(gids[7],67);  TEST_EQUALITY(gids[8],68);
      TEST_EQUALITY(gids[9],63); TEST_EQUALITY(gids[10],64); TEST_EQUALITY(gids[11],65);
   }
   else if(myRank==0) {
      // element 7
      const int * gids = connManager->getConnectivity(mesh->elementLocalId(7));
      TEST_EQUALITY(connManager->getConnectivitySize(mesh->elementLocalId(7)),8);

      TEST_EQUALITY(gids[0],12); TEST_EQUALITY(gids[1],13);  
      TEST_EQUALITY(gids[2],16); TEST_EQUALITY(gids[3],15);
      TEST_EQUALITY(gids[4],21); TEST_EQUALITY(gids[5],22);  
      TEST_EQUALITY(gids[6],25); TEST_EQUALITY(gids[7],24);
   }
   else if(myRank==1) {
      // element 2
      const int * gids = connManager->getConnectivity(mesh->elementLocalId(2));
      TEST_EQUALITY(connManager->getConnectivitySize(mesh->elementLocalId(2)),8);

      TEST_EQUALITY(gids[0],1); TEST_EQUALITY(gids[1],2);  
      TEST_EQUALITY(gids[2],5); TEST_EQUALITY(gids[3],4);
      TEST_EQUALITY(gids[4],10); TEST_EQUALITY(gids[5],11);  
      TEST_EQUALITY(gids[6],14); TEST_EQUALITY(gids[7],13);
   }
}

}
