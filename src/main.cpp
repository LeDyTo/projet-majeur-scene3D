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
bool start = false;
int W = 1080; int H = 720;


inline void parametreScene(bool screenChange, is::IMeshSceneNode *node, is::ISceneManager *smgr, std::vector<is::IAnimatedMesh*> meshVector,
                           scene::ITriangleSelector *selector, scene::ISceneNodeAnimator *anim, is::IAnimatedMeshSceneNode *perso,
                           core::vector3df radius, scene::ISceneNodeAnimator *animcam, scene::ICameraSceneNode* camera);

inline std::vector<iv::ITexture*> loadGif(int nbFrame, std::wstring nomGeneral, iv::IVideoDriver *driver);

inline void playVideo(std::vector<iv::ITexture*> frameVector, int nbFrame, ic::rect<s32> box, IrrlichtDevice *device,
                      is::ISceneManager *smgr, ig::IGUIEnvironment *gui, iv::IVideoDriver  *driver);

inline void title_Screen(ig::IGUIEnvironment *gui, iv::IVideoDriver  *driver, is::ISceneManager *smgr);

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
static void create_window(ig::IGUIEnvironment *gui)
{
  // Les fenêtres
    window = gui->addWindow(ic::rect<s32>(420,25, 620,460), false, L"items");
    ecranTitre = gui->addWindow(ic::rect<s32>(0,0, W,H), false, L"name");

    gui->addButton(ic::rect<s32>(40,74, 140,92), ecranTitre, WINDOW_BUTTON, L"Start");
}


/*===========================================================================*\
 * main                                                                      *
\*===========================================================================*/
int main()
{
  /// Le gestionnaire d'événements ///
  MyEventReceiver receiver;
  receiver.set_start(start);

  ////variables aléatoires pour lancement combat////
    float probaFight = 0.0005;
    bool isFight = false;
    float randNum;
    bool ScreenChange = false;

////ecran titre///////////

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
  create_window(gui);
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

  srand (time(NULL));
///// while loop /////

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));

//    if (!start)
//        title_Screen(gui, driver, smgr);


////combat hasard////////////////////////////////////////
/////////////////////////////////////////////////////////
    if (!isFight)
    {
        randNum = rand()/(float)RAND_MAX;

        ScreenChange = randNum < probaFight;
    }

    if (ScreenChange)
    {
        std::cout<<"ok"<<std::endl;
        isFight = true;

        playVideo(fightVector, nbFrameFight, fightBox, device, smgr, gui, driver);

        //node->remove();

        node2 = smgr->addOctreeSceneNode(meshVector[1]->getMesh(0), nullptr, 0, 1024);
        // Translation pour que nos personnages soient dans le décor
        node2->setPosition(core::vector3df(-1350, -180, -1400));

        perso->removeAnimator(anim);
        camera->removeAnimator(animcam);


        parametreScene(ScreenChange, node2, smgr, meshVector, selector2, anim2, perso, radius, animcam2, camera);
        ScreenChange = false;
    }
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

    if (!start && node && node2)
    {
        node->setVisible(false);
        node2->setVisible(false);
    }
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
inline void title_Screen(ig::IGUIEnvironment *gui, iv::IVideoDriver  *driver, is::ISceneManager *smgr)
{
    while(!start)
    {
        smgr->drawAll();
        gui->drawAll();
        driver->endScene();
    }
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
inline void titleScreen();

