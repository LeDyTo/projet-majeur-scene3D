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

  scene::ICameraSceneNode* camera =
  smgr->addCameraSceneNode(perso);
  camera->setPosition(ic::vector3df(-50, 30, 0));
  scene::ISceneNodeAnimator *animcam;
  scene::ISceneNodeAnimator *animcam2;




  //////////// physique //////////////
  // Création du triangle selector
  scene::ITriangleSelector *selector;
  scene::ITriangleSelector *selector2;



  parametreScene(ScreenChange, node, smgr, meshVector, selector, anim, perso, radius, animcam, camera);






///// Chargement des pieces ///
//is::IAnimatedMesh *meshCoin = smgr->getMesh("data/coin/coin.obj");


//int NbCoins = 10;
//is::IAnimatedMeshSceneNode *coins[NbCoins];


//for (int k = 0; k < NbCoins; k++)
//{
//    coins[k] = smgr->addAnimatedMeshSceneNode(meshCoin);
//    coins[k]->setMaterialFlag(iv::EMF_LIGHTING, false);
//    coins[k]->setMaterialTexture(0, driver->getTexture("data/coin/coin.png"));
//}

//for (int k = 0; k < NbCoins; k++)
//{
//    is::ISceneNodeAnimator *anim_cercle =
//                smgr->createRotationAnimator(ic::vector3df(0, 1, 0));
//    coins[k]->addAnimator(anim_cercle);
//}
//coins[0]->setPosition(ic::vector3df(131.569, -78.2488, -898.676));
//coins[1]->setPosition(ic::vector3df(884.863, 1.75118, -1437.7));
//coins[2]->setPosition(ic::vector3df(249.567, -142.249, -1703.5));
//coins[3]->setPosition(ic::vector3df(-740.726, 1.75118, -1509.24));
//coins[4]->setPosition(ic::vector3df(-300.306, -214.249, -1957.32));
//coins[5]->setPosition(ic::vector3df(685.036, 209.751, -1640.54));
//coins[6]->setPosition(ic::vector3df(235.582, -262.249, 652.879));
//coins[7]->setPosition(ic::vector3df(3.19827, -118.249, -2153.17));
//coins[8]->setPosition(ic::vector3df(-558.973, -198.249, -2396.41));
//coins[9]->setPosition(ic::vector3df(732.488, -137.793, -2357.42));




/////// ennemis /////
//is::IAnimatedMeshSceneNode *perso_cercle[NbCoins];

//for (int k = 0; k < NbCoins; k++)
//{
//    perso_cercle[k] = smgr->addAnimatedMeshSceneNode(mesh);
//    perso_cercle[k]->setMaterialFlag(iv::EMF_LIGHTING, false);
//    perso_cercle[k]->setMD2Animation(is::EMAT_RUN);
//    perso_cercle[k]->setMaterialTexture(0, driver->getTexture("data/tris/red_texture.pcx"));

//    is::ISceneNodeAnimator *anim_cercle =
//            smgr->createFlyStraightAnimator(coins[k]->getPosition(),
//                        coins[k]->getPosition() + ic::vector3df(100,0,60), 3500, true);

//    perso_cercle[k]->addAnimator(anim_cercle);
//}







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

//// Chargement des textures pour les chiffres
//iv::ITexture *digits[10];
//digits[0] = driver->getTexture("data/029/0.png");
//digits[1] = driver->getTexture("data/029/1.png");
//digits[2] = driver->getTexture("data/029/2.png");
//digits[3] = driver->getTexture("data/029/3.png");
//digits[4] = driver->getTexture("data/029/4.png");
//digits[5] = driver->getTexture("data/029/5.png");
//digits[6] = driver->getTexture("data/029/6.png");
//digits[7] = driver->getTexture("data/029/7.png");
//digits[8] = driver->getTexture("data/029/8.png");
//digits[9] = driver->getTexture("data/029/9.png");

//// Création des places pour les chiffres
//ig::IGUIImage *ttimeBox = gui->addImage(ic::rect<s32>(10,10,  50,50)); ttimeBox->setScaleImage(true);
//ig::IGUIImage *score_10000 = gui->addImage(ic::rect<s32>(50,10,  90,50)); score_10000->setScaleImage(true);
//ig::IGUIImage *score_1000  = gui->addImage(ic::rect<s32>(90,10,  130,50)); score_1000->setScaleImage(true);
//ig::IGUIImage *score_100   = gui->addImage(ic::rect<s32>(130,10,  170,50)); score_100->setScaleImage(true);







//// score
//iv::ITexture *tscore;
//tscore = driver->getTexture("data/coin/piece.png");
//iv::ITexture *tegal;
//tegal = driver->getTexture("data/coin/egal.png");
//ig::IGUIImage *tscoreBox = gui->addImage(ic::rect<s32>(W - 4*40,10,  W - 3*40,50)); tscoreBox->setScaleImage(true);
//ig::IGUIImage *tegalBox = gui->addImage(ic::rect<s32>(W - 3*40,10,  W - 2*40,50)); tegalBox->setScaleImage(true);
//ig::IGUIImage *score_10    = gui->addImage(ic::rect<s32>(W - 2*40,10,  W - 40,50)); score_10->setScaleImage(true);
//ig::IGUIImage *score_1     = gui->addImage(ic::rect<s32>(W - 40,10,  W,50)); score_1->setScaleImage(true);


////vie
//iv::ITexture *tvie;
//tvie = driver->getTexture("data/tris/vie.png");
//ig::IGUIImage *tvieBox = gui->addImage(ic::rect<s32>(W/2 - 3*40,10,  W/2 - 2*40,50)); tvieBox->setScaleImage(true);
//ig::IGUIImage *vie_1     = gui->addImage(ic::rect<s32>(W/2 - 40,10,  W/2,50)); vie_1->setScaleImage(true);


//// chrono
//iv::ITexture *ttime;
//ttime = driver->getTexture("data/029/chrono.png");


//// texture game over
//iv::ITexture *go;
//go = driver->getTexture("data/029/gameOver.png");
//// texture you win
//iv::ITexture *yw;
//yw = driver->getTexture("data/029/youWin.jpg");


//int health = 5;
//int time = 30000; // temps en secondes * 100 => 30000 = 300 s = 5 min
//int score = 0;
//std::vector<int> collected;






////variables aléatoires pour lancement combat////
  float probaFight = 0.0005;
  srand (time(NULL));
///// while loop /////

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));


    randNum = rand()/(float)RAND_MAX;

    ScreenChange = randNum < probaFight;

    if (ScreenChange)
    {
        std::cout<<"ok"<<std::endl;

        node->setVisible(false);

        node2 = smgr->addOctreeSceneNode(meshVector[1]->getMesh(0), nullptr, 0, 1024);
        // Translation pour que nos personnages soient dans le décor
        node2->setPosition(core::vector3df(-1350, -130, -1400));

        perso->removeAnimator(anim);
        camera->removeAnimator(animcam);


        parametreScene(ScreenChange, node2, smgr, meshVector, selector2, anim2, perso, radius, animcam2, camera);
        ScreenChange = false;
    }

    camera->setTarget(perso->getPosition());

    //std::cout << perso->getPosition().X << ", " <<  perso->getPosition().Y << ", " << perso->getPosition().Z << ", " <<std::endl;

    smgr->drawAll();


//    float epsilon = 10;

//    for (int k = 0; k < NbCoins; k++)
//    {
//        //std::cout << k << ": "<< irr::core::abs_(perso->getPosition().X - coins[k]->getPosition().X) << std::endl;
//        if(  (irr::core::abs_(perso->getPosition().X - coins[k]->getPosition().X) <= epsilon) &&
//             (irr::core::abs_(perso->getPosition().Y - coins[k]->getPosition().Y) <= epsilon) &&
//             (irr::core::abs_(perso->getPosition().Z - coins[k]->getPosition().Z) <= epsilon)
//          )
//        {
//            std::cout << "piece [" << k << "] : ok" << std::endl;
//            coins[k]->setPosition(ic::vector3df(1000000,100000,1000000));
//            score ++;
//            collected.push_back(k);
//        }

//    }



//    float epsilon2 = 10;

//    for (int k = 0; k < NbCoins; k++)
//    {
//        //std::cout << k << ": "<< irr::core::abs_(perso->getPosition().X - coins[k]->getPosition().X) << std::endl;
//        if(  (irr::core::abs_(perso->getPosition().X - perso_cercle[k]->getPosition().X) <= epsilon2) &&
//             (irr::core::abs_(perso->getPosition().Y - perso_cercle[k]->getPosition().Y) <= epsilon2) &&
//             (irr::core::abs_(perso->getPosition().Z - perso_cercle[k]->getPosition().Z) <= epsilon2)
//          )
//        {
//           // std::cout << k << ": ok" << std::endl;
//             health --;
//             std::cout << "health --" <<  std::endl;
//             perso->setPosition(ic::vector3df(9.19124, -94.2488, -1042.13));

//        }

//    }

////    for (unsigned int k = 0; k < collected.size() ; k++)
////    {
////        std::cout << "**************" << std::endl;
////        std::cout << collected[k] << std::endl;
////    }




//    /// Calcul du score :
//    time--;

//    // Mise à jour du temps :
//    ttimeBox->setImage(ttime);
//    score_10000->setImage(digits[(time / 10000) % 10]);
//    score_1000->setImage(digits[(time / 1000) % 10]);
//    score_100->setImage(digits[(time / 100) % 10]);

//    //score
//    score_10->setImage(digits[(score / 10) % 10]);
//    score_1->setImage(digits[(score / 1) % 10]);
//    tscoreBox->setImage(tscore);
//    tegalBox->setImage(tegal);


//    //vie
//    tvieBox->setImage(tvie);
//    vie_1->setImage(digits[(health / 1) % 10]);


////dessin de la barre de HP////

    hpBox->setImage(hpTexture);

    // Dessin de la GUI :
    gui->drawAll();











//    if (time == 99 || score == 10 || health == 0)
//    {
//        int best_time = time;

//        //std::cout << best_time << std::endl;

//        int best_health = health;
//        //time = 50000000;
//        int best_score = score;

//        int offset = 50;

//        ig::IGUIImage *tgo = gui->addImage(ic::rect<s32>(0,0,  W,H)); tgo->setScaleImage(true);

//        if (time >= 99 && score == 10 && health != 0)
//            tgo->setImage(yw); // you win
//        else if (!(time >= 99 && score == 10 && health != 0))
//            tgo->setImage(go); // game over




//        // score game over
//        ig::IGUIImage *tscoreBox = gui->addImage(ic::rect<s32>(W/2 - 4*40, H/2 + 10 +offset,  W/2 - 3*40,H/2 + 50+offset)); tscoreBox->setScaleImage(true);
//        ig::IGUIImage *tegalBox = gui->addImage(ic::rect<s32>(W/2 - 3*40,H/2 + 10+offset,  W/2 - 2*40,H/2 + 50+offset)); tegalBox->setScaleImage(true);
//        ig::IGUIImage *score_10    = gui->addImage(ic::rect<s32>(W/2 - 2*40,H/2 + 10+offset,  W/2 - 40,H/2 + 50+offset)); score_10->setScaleImage(true);
//        ig::IGUIImage *score_1     = gui->addImage(ic::rect<s32>(W/2 - 40,H/2 + 10+offset,  W/2,H/2 + 50+offset)); score_1->setScaleImage(true);

//        score_10->setImage(digits[(best_score / 10) % 10]);
//        score_1->setImage(digits[(best_score / 1) % 10]);
//        tscoreBox->setImage(tscore);
//        tegalBox->setImage(tegal);





//        // health game over
//        ig::IGUIImage *thealthBox = gui->addImage(ic::rect<s32>(W/2 - 4*40, H/2 + 50+offset,  W/2 - 3*40,H/2 + 90+offset)); thealthBox->setScaleImage(true);
//        ig::IGUIImage *health_1     = gui->addImage(ic::rect<s32>(W/2 - 2*40, H/2 + 50+offset,  W/2 - 1*40,H/2 + 90+offset)); health_1->setScaleImage(true);

//        health_1->setImage(digits[(best_health / 1) % 10]);
//        thealthBox->setImage(tvie);


//        // time game over
//        ig::IGUIImage *ttimeBox = gui->addImage(ic::rect<s32>(W/2 - 4*40, H/2 + 90 +offset,  W/2 - 3*40,H/2 + 130+offset)); ttimeBox->setScaleImage(true);
//        ig::IGUIImage *time_10000    = gui->addImage(ic::rect<s32>(W/2 - 2*40,H/2 + 90+offset,  W/2 - 40,H/2 + 130+offset)); time_10000->setScaleImage(true);
//        ig::IGUIImage *time_1000     = gui->addImage(ic::rect<s32>(W/2 - 40,H/2 + 90+offset,  W/2,H/2 + 130+offset)); time_1000->setScaleImage(true);
//        ig::IGUIImage *time_100     = gui->addImage(ic::rect<s32>(W/2 ,H/2 + 90+offset,  W/2 + 40,H/2 + 130+offset)); time_100->setScaleImage(true);



//        ttimeBox->setImage(ttime);
//        time_10000->setImage(digits[(best_time / 10000) % 10]);
//        time_1000->setImage(digits[(best_time / 1000) % 10]);
//        time_100->setImage(digits[(best_time / 100) % 10]);






//    }


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
