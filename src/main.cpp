/**************** Includes ****************/

#include <irrlicht.h>

#include "../lib/MyEventReceiver.hpp"

#include <vector>

/**************** Namespaces ****************/

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

inline void parametreScene(bool screenChange, is::IMeshSceneNode *node, is::ISceneManager *smgr, std::vector<is::IAnimatedMesh*> meshVector,
                           scene::ITriangleSelector *selector, scene::ISceneNodeAnimator *anim, is::IAnimatedMeshSceneNode *perso,
                           core::vector3df radius, scene::ISceneNodeAnimator *animcam, scene::ICameraSceneNode* camera);

int main()
{
  /// Le gestionnaire d'événements ///
  MyEventReceiver receiver;

  float randNum;
  bool ScreenChange = false;

  // Création de la fenêtre et du système de rendu.
  int W = 1080; int H = 720;
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(W, H),
                                        16, false, false, false, &receiver);
  // nom de l'application
  core::stringw tmp = L"My Game";
  device->setWindowCaption(tmp.c_str());



  // Joystick Activation & Debug
  receiver.joystick1.myJoysticksActivation(device);



  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();
  is::IMeshSceneNode *node;
  is::IMeshSceneNode *node2;


  /// on charge le decor ///
  // Ajout de l'archive qui contient entre autres un niveau complet
  device->getFileSystem()->addFileArchive("data/maps/mario.pk3");
  device->getFileSystem()->addFileArchive("data/maps/map-20kdm2.pk3");

  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *mesh_bsp = smgr->getMesh("mario.bsp");
  is::IAnimatedMesh *mesh_bsp2 = smgr->getMesh("20kdm2.bsp");
  std::vector<is::IAnimatedMesh*> meshVector;
  meshVector.push_back(mesh_bsp);
  meshVector.push_back(mesh_bsp2);

  node = smgr->addOctreeSceneNode(meshVector[0]->getMesh(0), nullptr, -1, 1024);
  // Translation pour que nos personnages soient dans le décor
  node->setPosition(core::vector3df(0,-104,0));



  /// Chargement de notre personnage ///
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris/tris.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode *perso = smgr->addAnimatedMeshSceneNode(mesh);
  receiver.perso1 = perso;
  perso->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso->setMD2Animation(is::EMAT_STAND);
  perso->setMaterialTexture(0, driver->getTexture("data/tris/blue_texture.pcx"));
  perso->setRotation(ic::vector3df(0, 90, 0));


  const core::aabbox3d<f32>& box = perso->getBoundingBox();
  core::vector3df radius = box.MaxEdge - box.getCenter();
  scene::ISceneNodeAnimator *anim;
  scene::ISceneNodeAnimator *anim2;



  ///////////// Camera //////////////

  scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(perso);
  camera->setPosition(ic::vector3df(-50, 30, 0));
  scene::ISceneNodeAnimator *animcam;
  scene::ISceneNodeAnimator *animcam2;




  //////////// physique //////////////
  // Création du triangle selector
  scene::ITriangleSelector *selector;
  scene::ITriangleSelector *selector2;



  parametreScene(ScreenChange, node, smgr, meshVector, selector, anim, perso, radius, animcam, camera);


////// images 2D ////

//  iv::IImageLoader *gifLoader;
//  driver->addExternalImageLoader(gifLoader);


///// barre de HP ////
//  irr::io::IReadFile *health;
//  iv::IImage *hpBar = gifLoader->loadImage(health);
  iv::ITexture *hpTexture = driver->getTexture("data/perso/hp_png/frame-01.png");

//  for(int i = 0; i < 59; ++i)
//  {
//     health = irr::io::createReadFile("data/perso/hp_png/frame-01.png");
//     hpBar = gifLoader->loadImage(health);
//     hpTexture[0] = driver->addTexture("hpTexture",hpBar);
//  }

//// attribution de place pour la barre de H////
  ig::IGUIImage *hpBox = gui->addImage(ic::rect<s32>(10,10,  50,50)); hpBox->setScaleImage(true);

/////////////fonction inline lecture de GIF//////////////////

//  inline std::vector<irr::video::IImage*>  load(const char*file)
//  {
//           irr::core::array<irr::video::IImage*> calque;
//           int i, ExtCode;
//           GifFileType *File;
//           File = DGifOpenFileName(file);

//           for(int i = 0; i < File->ImageCount; ++i);
//           {
//               irr::video::IImage *tmp = driver->createImageFromData(irr::video::ECF_A8R8G8B8,irr::core::dimension2d<irr::u32>((irr::u32)File->SWidth,(irr::u32)File->SHeight),(void*)(File->SColorMap[i].Colors),false);
//               calque.push_back(tmp);
//           }

//           DGifCloseFile(File);
//           return true;
//  }

////variables aléatoires pour lancement combat////
  float probaFight = 0.0005;
  bool isFight = false;
  srand (time(NULL));
///// while loop /////

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));

////combat hasard/////
//    if (!isFight)
//    {
//        randNum = rand()/(float)RAND_MAX;

//        ScreenChange = randNum < probaFight;
//    }

//    if (ScreenChange)
//    {
//        std::cout<<"ok"<<std::endl;
//        isFight = true;

//        node->setVisible(false);

//        node2 = smgr->addOctreeSceneNode(meshVector[1]->getMesh(0), nullptr, 0, 1024);
//        // Translation pour que nos personnages soient dans le décor
//        node2->setPosition(core::vector3df(-1350, -130, -1400));

//        perso->removeAnimator(anim);
//        camera->removeAnimator(animcam);


//        parametreScene(ScreenChange, node2, smgr, meshVector, selector2, anim2, perso, radius, animcam2, camera);
//        ScreenChange = false;
//    }
/////////////////////////
    camera->setTarget(perso->getPosition());

    smgr->drawAll();

////dessin de la barre de HP////

    hpBox->setImage(hpTexture);

    // Dessin de la GUI :
    gui->drawAll();

    driver->endScene();
  }
  device->drop();

  return 0;
}

inline void parametreScene(bool screenChange, is::IMeshSceneNode *node, is::ISceneManager *smgr, std::vector<is::IAnimatedMesh*> meshVector,
                           scene::ITriangleSelector *selector, scene::ISceneNodeAnimator *anim, is::IAnimatedMeshSceneNode *perso,
                           core::vector3df radius, scene::ISceneNodeAnimator *animcam, scene::ICameraSceneNode* camera)
{
    std::cout<<"ok par"<<std::endl;

//    if (node)
//    {
//        node->setVisible(false);
//    }

//    if (screenChange)
//    {
//    node = smgr->addOctreeSceneNode(meshVector[1]->getMesh(0), nullptr, 0, 1024);
//    // Translation pour que nos personnages soient dans le décor
//    node->setPosition(core::vector3df(-1350, -130, -1400));
//    }

//    else
//    {
//        node = smgr->addOctreeSceneNode(meshVector[0]->getMesh(0), nullptr, -1, 1024);
//        // Translation pour que nos personnages soient dans le décor
//        node->setPosition(core::vector3df(0,-104,0));
//    }

    selector = smgr->createOctreeTriangleSelector(node->getMesh(), node);
    node->setTriangleSelector(selector);


    anim = smgr->createCollisionResponseAnimator(selector,
                                                 perso,  // Le noeud que l'on veut gérer
                                                 radius, // "rayons" de la caméra
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, 0, 0));  // décalage du centre

    perso->addAnimator(anim);





    animcam = smgr->createCollisionResponseAnimator(selector,
                                                 camera,
                                                 ic::vector3df(30, 50, 30),
                                                 ic::vector3df(0, 0, 0),  // gravité
                                                 ic::vector3df(0, 0, 0));  // décalage du centre

    camera->addAnimator(animcam);


}
