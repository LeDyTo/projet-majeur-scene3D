#include "MyEventReceiver.hpp"

namespace ig = irr::gui;

bool MyEventReceiver::OnEvent(const irr::SEvent &event)
  {



    /********** Clavier, souris et gui(menu et fenetre) ***********/
    if (!perso1) return false;
    switch (event.EventType)
    {
      case irr::EET_KEY_INPUT_EVENT:
        return keyboard(event);
      case irr::EET_MOUSE_INPUT_EVENT:
        return mouse(event);
      case irr::EET_GUI_EVENT:
        return gui_manage(event);
      default:;
    }

    /********** Joystick ***********/
    joystick1.perso = perso1;
    joystick1.cam = cam1;
    joystick1.controllerEvent(event);

    return false;
  }







bool MyEventReceiver::keyboard(const irr::SEvent &event)
{
    static int isMoving = 0;
    static int hasJumped = 0;

    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {


        if(event.KeyInput.PressedDown && event.KeyInput.Key == irr::KEY_SPACE)
            hasJumped += 1;
        else if (!event.KeyInput.PressedDown || event.KeyInput.Key == irr::KEY_SPACE)
            hasJumped = 0;


        if(event.KeyInput.PressedDown)
        {
            //std::cout << perso1->getPosition().X <<  ", " <<  perso1->getPosition().Y <<  ", " <<  perso1->getPosition().Z << std::endl;

            irr::core::vector3df position = perso1->getPosition();
            irr::core::vector3df rotation = perso1->getRotation();
            switch (event.KeyInput.Key)
            {
              case irr::KEY_ESCAPE:
                exit(0);
              case irr::KEY_KEY_Z: // Avance
                position.X += 1 * cos(rotation.Y * M_PI / 180.0) * speed;
                position.Z += -1 * sin(rotation.Y * M_PI / 180.0) * speed;
                isMoving += 1;
                break;
              case irr::KEY_KEY_S: // Recule
                position.X += -1 * cos(rotation.Y * M_PI / 180.0) * speed;
                position.Z += 1 * sin(rotation.Y * M_PI / 180.0) * speed;
                isMoving += 1;
                break;
              case irr::KEY_KEY_D: // Tourne à droite
                rotation.Y += 10;
                isMoving += 1;
                break;
              case irr::KEY_KEY_Q: // Tourne à gauche
                rotation.Y -= 10;
                isMoving += 1;
                break;
              case irr::KEY_KEY_A: // Tourne à gauche
                speed = (float)(((int)speed )%18) + 3;
                std::cout << "speed = " << speed << std::endl;
                break;
            case irr::KEY_KEY_I: // Tourne à gauche
                window->setVisible(isOpened);
                isOpened = !isOpened;
                break;
              default:
                isMoving = 0;
            }
            perso1->setPosition(position);
            perso1->setRotation(rotation);
        }

        else if(!event.KeyInput.PressedDown)
            isMoving = 0;

        if (isMoving == 1)
          perso1->setMD2Animation(irr::scene::EMAT_RUN);
        if (isMoving == 0)
           perso1->setMD2Animation(irr::scene::EMAT_STAND);
        if (hasJumped == 1)
          perso1->setMD2Animation(irr::scene::EMAT_JUMP);

    }


  return false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::mouse(const irr::SEvent &event)
{
  switch(event.MouseInput.Event)
  {
    case irr::EMIE_LMOUSE_PRESSED_DOWN:
      button_pressed = true;
      old_x = event.MouseInput.X;
      old_y = event.MouseInput.Y;
      break;
    case irr::EMIE_LMOUSE_LEFT_UP:
      button_pressed = false;
      break;
    case irr::EMIE_MOUSE_MOVED:
      if (button_pressed)
      {
        irr::core::vector3df rotation = perso1->getRotation();
        rotation.Y -= (event.MouseInput.X - old_x);
        old_x = event.MouseInput.X;
        old_y = event.MouseInput.Y;
        perso1->setRotation(rotation);
      }
      break;
    default:
      ;
  }

  return false;
}


/*------------------------------------------------------------------------*\
 * fenetre et menu                                                        *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::gui_manage(const irr::SEvent &event)
{
  if (!node) return false;
  switch(event.GUIEvent.EventType)
  {
    // Gestion des menus de la barre de menu
    case ig::EGET_MENU_ITEM_SELECTED:
      {
        ig::IGUIContextMenu *menu = (ig::IGUIContextMenu*)event.GUIEvent.Caller;
        irr::s32 item = menu->getSelectedItem();
        irr::s32 id = menu->getItemCommandId(item);

        switch(id)
        {
          case MENU_NEW_GAME:
            // Faire quelque chose ici !
            break;
          case MENU_QUIT:
            exit(0);
          case MENU_COMMANDES:
            gui->addMessageBox(L"Commandes", L"En exploration, ....");
            break;
        }
      }
      case ig::EGET_BUTTON_CLICKED:
        {
          irr::s32 id = event.GUIEvent.Caller->getID();
          if (id == WINDOW_BUTTON)
          {
              start = true;
          }
          if (id == ITEM_1)
          {
              window->getElementFromId(id)->setVisible(false);
              gainHp = 0.4;
          }
          if (id == ITEM_2)
          {
              window->getElementFromId(id)->setVisible(false);
              gainHp = 0.4;
          }
          if (id == ITEM_3)
          {
              window->getElementFromId(id)->setVisible(false);
              gainHp = 0.4;
          }
          if (id == ITEM_4)
          {
              window->getElementFromId(id)->setVisible(false);
              gainHp = 0.4;
          }
          if (id == ITEM_5)
          {
              window->getElementFromId(id)->setVisible(false);
              gainHp = 0.65;
          }
          if (id == ITEM_6)
          {
              window->getElementFromId(id)->setVisible(false);
              gainHp = 0.65;
          }
          if (id == ITEM_7)
          {
              window->getElementFromId(id)->setVisible(false);
              gainHp = 0.90;
          }
          if (id == ITEM_8)
          {
              window->getElementFromId(id)->setVisible(false);
          }
       }
        break;
        default:
      ;
   break;
  }
  return false;
}

/**************************************************************************\
 * MyEventReceiver::set_node                                                *
\**************************************************************************/
void MyEventReceiver::set_node(irr::scene::ISceneNode *n)
{
  node = n;
}

/**************************************************************************\
 * MyEventReceiver::set_gui                                                 *
\**************************************************************************/
void MyEventReceiver::set_gui(irr::gui::IGUIEnvironment *g)
{
  gui = g;
}

/*===========================================================================*\
 * set_window for items                                                      *
\*===========================================================================*/
void MyEventReceiver::set_window(ig::IGUIWindow *w)
{
  window = w;
}

/*===========================================================================*\
 * get_start game                                                    *
\*===========================================================================*/
bool MyEventReceiver::get_start()
{
  return start;
}

/*===========================================================================*\
 * get hp++                                                                  *
\*===========================================================================*/
int MyEventReceiver::applyGainHp(int hp, int hpMax)
{

  hp += gainHp*(float)(hpMax - hp);
  gainHp = 0;
  std::cout << hp << std::endl;

  return hp;
}

MyEventReceiver::MyEventReceiver():
    perso1(nullptr), button_pressed(false)
{}
