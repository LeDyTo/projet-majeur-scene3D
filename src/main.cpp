/**************** Includes ****************/

#include <irrlicht.h>

#include "../lib/MyEventReceiver.hpp"
#include "../lib/gui_ids.h"

#include <vector>
#include <string>


/**************** Namespaces ****************/

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

int hp = 89;
const int hpmax = 200;
ig::IGUIWindow *window;
ig::IGUIWindow *ecranTitre;
int W = 1080; int H = 720;


inline void parametreScene(bool screenChange, is::IMeshSceneNode *node, is::ISceneManager *smgr, std::vector<is::IAnimatedMesh*> meshVector,
                           scene::ITriangleSelector *selector, scene::ISceneNodeAnimator *anim, is::IAnimatedMeshSceneNode *perso,
                           core::vector3df radius, scene::ISceneNodeAnimator *animcam, scene::ICameraSceneNode* camera);

inline std::vector<iv::ITexture*> loadGif(int nbFrame, std::wstring nomGeneral, iv::IVideoDriver *driver);

inline void playVideo(std::vector<iv::ITexture*> frameVector, int nbFrame, ic::rect<s32> box, IrrlichtDevice *device,
                      is::ISceneManager *smgr, ig::IGUIEnvironment *gui, iv::IVideoDriver  *driver);

inline void title_Screen(is::IMeshSceneNode *node, is::IMeshSceneNode *node2, is::IAnimatedMeshSceneNode *perso, ig::IGUIImage *hpBox);

inline void   parametreChest(is::ISceneManager *smgr, int NbChest, iv::IVideoDriver *driver, is::IAnimatedMeshSceneNode **chest,
                             iv::ITexture **items, s32 *idItem, is::IAnimatedMesh *meshChest);

inline void openChest(is::IAnimatedMeshSceneNode **chest, is::IAnimatedMeshSceneNode *perso, ig::IGUIButton **itemsButton,
                      ig::IGUIEnvironment *gui, ig::IGUIWindow *window, s32 *idItem, int nbObjetTrouve, MyEventReceiver receiver,
                      int NbChest, iv::ITexture **items);
/*===========================================================================*\
 * create_menu                                                               *
\*===========================================================================*/
static void create_menu(ig::IGUIEnvironment *gui)
{
  ig::IGUIContextMenu *submenu;

  // une entrée principale :
  ig::IGUIContextMenu *menu = gui->addMenu();
  menu->addItem(L"menu", -1, true, true);

  // Le contenu du menu :
  submenu = menu->getSubMenu(0);
  submenu->addItem(L"New game...", MENU_NEW_GAME);
  submenu->addSeparator();
  submenu->addItem(L"Quit", MENU_QUIT);
  submenu->addSeparator();
  submenu->addItem(L"commandes", MENU_COMMANDES);
}

/*===========================================================================*\
 * create_window for items                                                   *
\*===========================================================================*/
static void create_window(ig::IGUIEnvironment *gui, iv::IVideoDriver  *driver)
{
  //fenetre de l inventaire
    window = gui->addWindow(ic::rect<s32>(10, 70, W-10,H-30), false, L"items");window->setDraggable(false);window->setDrawTitlebar(false);
    iv::ITexture *fondItem = driver->getTexture(L"data/potions/fondItem.png");
    gui->addImage(fondItem, ic::position2d<s32>(0, 0), true, window);

    //ecran titre
    ecranTitre = gui->addWindow(ic::rect<s32>(0, 0, W,H), false); ecranTitre->setDrawTitlebar(false); ecranTitre->setDraggable(false);
    iv::ITexture *name = driver->getTexture(L"data/menu/ecrantitre.png");
    iv::ITexture *start = driver->getTexture(L"data/menu/startbutton.png");
    gui->addImage(name, ic::position2d<s32>(0, 0), true, ecranTitre);
    ig::IGUIButton *startButton = gui->addButton(ic::rect<s32>(W/2 - 150, H/2 - 50, W/2 + 150, H/2 + 50), ecranTitre, WINDOW_BUTTON);
    startButton->setUseAlphaChannel(true);startButton->setDrawBorder(false);
    startButton->setImage(start); startButton->setScaleImage(true);
}


/*===========================================================================*\
 * main                                                                      *
\*===========================================================================*/
int main()
{
  /// Le gestionnaire d'événements ///
  MyEventReceiver receiver;

  ////variables aléatoires pour lancement combat////
    float probaFight = 0.0005;
    bool isFight = false;
    float randNum;
    bool ScreenChange = false;

    int NbChest = 7;
    ig::IGUIButton *itemsButton[NbChest+1];

    int nbObjetTrouve = 0;
    bool objetTrouve = true;

////numero de frame pour affichage hp///////////
  int nbFrameHp = 60;
  int numCurrentFrame;

  // Création de la fenêtre et du système de rendu.
  int W = 1080; int H = 720;
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(W, H),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();
  is::IMeshSceneNode *node;
  is::IMeshSceneNode *node2;

  // La barre de menu
  create_menu(gui);

  // fenêtre des objets et ecran titre
  create_window(gui, driver);
  window->setVisible(false);

  //liste des images barre de hp
  std::wstring nomGeneralHp(L"data/perso/hp/health");

  std::vector<iv::ITexture*> hpVector = loadGif(nbFrameHp, nomGeneralHp, driver);

  ig::IGUIImage *hpBox = gui->addImage(ic::rect<s32>(10,25,  300,40)); hpBox->setScaleImage(true);


  //animation debut combat
  std::wstring nomGeneralFight(L"data/animations/combat/combat");
  int nbFrameFight = 20;
  std::vector<iv::ITexture*> fightVector = loadGif(nbFrameHp, nomGeneralFight, driver);

  ic::rect<s32> fightBox(0,  0, W, H);

  // nom de l'application
  core::stringw tmp = L"My Game";
  device->setWindowCaption(tmp.c_str());



  // Joystick Activation & Debug
  receiver.joystick1.myJoysticksActivation(device);


  /// on charge le decor ///
  // Ajout de l'archive qui contient entre autres un niveau complet
  device->getFileSystem()->addFileArchive("data/maps/cf.pk3");
  device->getFileSystem()->addFileArchive("data/maps/map-20kdm2.pk3");

  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *mesh_bsp = smgr->getMesh("cf.bsp");
  is::IAnimatedMesh *mesh_bsp2 = smgr->getMesh("20kdm2.bsp");
  std::vector<is::IAnimatedMesh*> meshVector;
  meshVector.push_back(mesh_bsp);
  meshVector.push_back(mesh_bsp2);

  node = smgr->addOctreeSceneNode(meshVector[0]->getMesh(0), nullptr, -1, 1024);
  node2 = smgr->addOctreeSceneNode(meshVector[1]->getMesh(0), nullptr, 0, 1024);

  // Translation pour que nos personnages soient dans le décor
  node->setPosition(core::vector3df(200,-200,250));


  /// Chargement de notre personnage ///
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris/tris.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode *perso = smgr->addAnimatedMeshSceneNode(mesh);
  receiver.perso1 = perso;
  perso->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso->setMD2Animation(is::EMAT_STAND);
  perso->setMaterialTexture(0, driver->getTexture("data/tris/blue_texture.pcx"));
  perso->setRotation(ic::vector3df(0, 90, 0));
  perso->setPosition(ic::vector3df(1898.98, 450.025, -1539.27));

  //on set tout ce qui faut pour le receiver
  receiver.set_gui(gui);
  receiver.set_node(node);
  receiver.set_window(window);

  //perso
  const core::aabbox3d<f32>& box = perso->getBoundingBox();
  core::vector3df radius = box.MaxEdge - box.getCenter();
  scene::ISceneNodeAnimator *anim;
  scene::ISceneNodeAnimator *anim2;

/////police de caractere///////
  ig::IGUISkin* skin = gui->getSkin();
  ig::IGUIFont* font = gui->getFont("data/menu/fontlucida.png");
  skin->setFont(font);

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


  /// Chargement des coffres ///


  is::IAnimatedMesh *meshChest = smgr->getMesh("data/coffre/Chest.obj");

  is::IAnimatedMeshSceneNode *chest[NbChest];
  iv::ITexture *items[NbChest + 1]; // vecteur texture pour fenetre d inventaire
  s32 idItem[NbChest + 1];
  parametreChest(smgr,NbChest,driver,chest,items, idItem, meshChest);



  srand (time(NULL));
///// while loop /////

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));
    hp = receiver.applyGainHp(hp,hpmax);

    if (receiver.get_start())
    {
        ecranTitre->setVisible(false);
        node->setVisible(true);
        node2->setVisible(true);
        perso->setVisible(true);
        hpBox->setVisible(true);
    }
    else
        title_Screen(node, node2, perso, hpBox);

//on verifie si on ouvre un coffre
    //std::cout<<"test                "<<receiver.get_interact()<<std::endl;
    openChest(chest, perso,itemsButton, gui, window, idItem, nbObjetTrouve, receiver, NbChest, items);

////combat hasard////////////////////////////////////////
/////////////////////////////////////////////////////////
//    if (!isFight && receiver.get_start())
//    {
//        randNum = rand()/(float)RAND_MAX;

//        ScreenChange = randNum < probaFight;
//    }

//    if (ScreenChange)
//    {
//        std::cout<<"ok"<<std::endl;
//        isFight = true;

//        playVideo(fightVector, nbFrameFight, fightBox, device, smgr, gui, driver);

//        node->setVisible(false);

//        node2 = smgr->addOctreeSceneNode(meshVector[1]->getMesh(0), nullptr, 0, 1024);
//        // Translation pour que nos personnages soient dans le décor
//        node2->setPosition(core::vector3df(-1350, -180, -1400));

//        perso->removeAnimator(anim);
//        camera->removeAnimator(animcam);


//        parametreScene(ScreenChange, node2, smgr, meshVector, selector2, anim2, perso, radius, animcam2, camera);
//        ScreenChange = false;
//    }
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

    camera->setTarget(perso->getPosition());

    smgr->drawAll();

////dessin de la barre de HP////

    numCurrentFrame = (nbFrameHp - 1) * (float)hp/hpmax;

    hpBox->setImage(hpVector[numCurrentFrame]);

    // Dessin de la GUI :
    gui->drawAll();

    driver->endScene();
  }
  device->drop();

  return 0;
}

/*===========================================================================*\
 * gestion sur l ecran titre                                                 *
\*===========================================================================*/
inline void title_Screen(is::IMeshSceneNode *node, is::IMeshSceneNode *node2, is::IAnimatedMeshSceneNode *perso, ig::IGUIImage *hpBox)
{
    node->setVisible(false);
    node2->setVisible(false);
    perso->setVisible(false);
    hpBox->setVisible(false);
}


/*===========================================================================*\
 * parametrisation des coffres                                               *
\*===========================================================================*/
inline void   parametreChest(is::ISceneManager *smgr, int NbChest, iv::IVideoDriver *driver, is::IAnimatedMeshSceneNode **chest,
                             iv::ITexture **items, s32 *idItem, is::IAnimatedMesh *meshChest)

{
    for (int k = 0; k < NbChest; k++)
    {
        chest[k] = smgr->addAnimatedMeshSceneNode(meshChest);
        chest[k]->setMaterialFlag(iv::EMF_LIGHTING, false);
        chest[k]->setMaterialTexture(0, driver->getTexture("data/coffre/diffuse.tga"));
    }
  //  const core::aabbox3d<f32>& boxChest = chest[0]->getBoundingBox();
  //  core::vector3df radiusChest = boxChest.MaxEdge - boxChest.getCenter();
  //  //on applique le poids et les colisions aux coffres
  //  for (int k = 0; k < 1; k++)
  //  {
  //      scene::ITriangleSelector *selectorChest = smgr->createOctreeTriangleSelector(node->getMesh(), node);
  //      node->setTriangleSelector(selectorChest);

  //      scene::ISceneNodeAnimator *animChest
  //                                = smgr->createCollisionResponseAnimator(selectorChest,
  //                                                                       chest[k],  // Le noeud que l'on veut gérer
  //                                                                       radiusChest, // "rayons" de la caméra
  //                                                                       ic::vector3df(0, -10, 0),  // gravité
  //                                                                       ic::vector3df(0, 0, 0));  // décalage du centre
  //      chest[k]->addAnimator(animChest);
  //  }
    chest[0]->setPosition(ic::vector3df(1898.98, 376.025, -1539.27));
    chest[0]->setRotation(ic::vector3df(0, 180, 0));
    items[0] = driver->getTexture("data/potions/petitePotion.png");
    idItem[0] = ITEM_1;

    chest[1]->setPosition(ic::vector3df(775.95, -8, -1767.98));
    items[1] = driver->getTexture("data/potions/petitePotion.png");
    idItem[1] = ITEM_2;

    chest[2]->setPosition(ic::vector3df(624.709, -183.975, -1358.84));
    chest[2]->setRotation(ic::vector3df(0, -44, 0));
    items[2] = driver->getTexture("data/potions/petitePotion.png");
    idItem[2] = ITEM_3;

    chest[3]->setPosition(ic::vector3df(424.272, 432.025, -966));
    chest[3]->setRotation(ic::vector3df(0, 90, 0));
    items[3] = driver->getTexture("data/potions/petitePotion.png");
    idItem[3] = ITEM_4;

    chest[4]->setPosition(ic::vector3df(-504, 568.025, 698.047));
    chest[4]->setRotation(ic::vector3df(0, 180, 0));
    items[4] = driver->getTexture("data/potions/moyennePotion.png");
    idItem[4] = ITEM_5;

    chest[5]->setPosition(ic::vector3df(263.147, 568.017, 609.818));
    chest[5]->setRotation(ic::vector3df(0, 180, 0));
    items[5] = driver->getTexture("data/potions/moyennePotion.png");
    idItem[5] = ITEM_6;

    chest[6]->setPosition(ic::vector3df(582.848, 952.02, 179.737));
    chest[6]->setRotation(ic::vector3df(0, -90, 0));
    items[6] = driver->getTexture("data/potions/grossePotion.png");
    idItem[6] = ITEM_7;

    items[7] = driver->getTexture("data/potions/grossePotion.png");
    idItem[7] = ITEM_8;

}


/*===========================================================================*\
 * update scene                                                              *
\*===========================================================================*/

inline void parametreScene(bool screenChange, is::IMeshSceneNode *node, is::ISceneManager *smgr, std::vector<is::IAnimatedMesh*> meshVector,
                           scene::ITriangleSelector *selector, scene::ISceneNodeAnimator *anim, is::IAnimatedMeshSceneNode *perso,
                           core::vector3df radius, scene::ISceneNodeAnimator *animcam, scene::ICameraSceneNode* camera)
{
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

/*===========================================================================*\
 * load gif                                                                  *
\*===========================================================================*/

inline std::vector<iv::ITexture*> loadGif(int nbFrame, std::wstring nomGeneral, iv::IVideoDriver *driver)
{
    std::vector<iv::ITexture*> Vector;
    std::wstring numFrame;
    std::wstring nomComplet;

    iv::ITexture *Texture;

    for(int i = 1; i<=nbFrame ; i++)
    {
      numFrame = std::to_wstring(i) + L".png";
      nomComplet = nomGeneral + numFrame;
      Texture = driver->getTexture(nomComplet.c_str());
      Vector.push_back(Texture);
    }
    return Vector;
}

/*===========================================================================*\
 * play video                                                                *
\*===========================================================================*/

inline void playVideo(std::vector<iv::ITexture*> frameVector, int nbFrame, ic::rect<s32> box, IrrlichtDevice *device,
                      is::ISceneManager *smgr, ig::IGUIEnvironment *gui, iv::IVideoDriver  *driver)
{
    ig::IGUIImage *Box = gui->addImage(box); Box->setScaleImage(true);

    int currentFrame = 0;
    while(currentFrame<nbFrame)
    {
        Box->setImage(frameVector[currentFrame]);
        currentFrame++;
        std::cout << currentFrame << std::endl;
        smgr->drawAll();
        gui->drawAll();
        driver->endScene();

        device->sleep(100);
    }
    Box->remove();
}



/*===========================================================================*\
 * is a chest opening                                                        *
\*===========================================================================*/
inline void openChest(is::IAnimatedMeshSceneNode **chest, is::IAnimatedMeshSceneNode *perso, ig::IGUIButton **itemsButton,
                      ig::IGUIEnvironment *gui, ig::IGUIWindow *window, s32 *idItem, int nbObjetTrouve, MyEventReceiver receiver,
                      int NbChest, iv::ITexture **items)
{
    int epsilon = 10;
    for (unsigned int k = 0; k < NbChest; k++)
    {
        if (chest[k] != NULL && perso != NULL) // pour eviter les erreurs de segmentations
        {

                if (    (core::abs_(perso->getPosition().X - chest[k]->getPosition().X)) <= epsilon
                        &&   (core::abs_(perso->getPosition().Y - chest[k]->getPosition().Y)) <= epsilon*5
                        &&   (core::abs_(perso->getPosition().Z - chest[k]->getPosition().Z)) <= epsilon
                        &&   receiver.get_interact() )

                {
                    //actualisation de l inventaire

                    std::cout << "get interact" << receiver.get_interact() << std::endl;
                            int ligne = nbObjetTrouve>3;
                            if (ligne ==0)
                                itemsButton[nbObjetTrouve] = gui->addButton(ic::rect<s32>((nbObjetTrouve%4)*1060/4 + 10, ligne*620/2 + 45,
                                                                                          (nbObjetTrouve%4 + 1)*1060/4 - 10, (ligne + 1)*620/2),
                                                                                          window, idItem[nbObjetTrouve]);
                            else
                                itemsButton[nbObjetTrouve] = gui->addButton(ic::rect<s32>((nbObjetTrouve%4)*1060/4 + 10, ligne*620/2,
                                                                                          (nbObjetTrouve%4 + 1)*1060/4 - 10, (ligne + 1)*620/2),
                                                                                          window, idItem[nbObjetTrouve]);

                            itemsButton[nbObjetTrouve]->setUseAlphaChannel(true);itemsButton[nbObjetTrouve]->setDrawBorder(false);
                            itemsButton[nbObjetTrouve]->setImage(items[nbObjetTrouve]);itemsButton[nbObjetTrouve]->setScaleImage(true);
                            nbObjetTrouve++;
                            std::cout << "nbobjet trouve " << nbObjetTrouve << std::endl;
                            chest[k]->setVisible(false);

                }
        }
    }
}
