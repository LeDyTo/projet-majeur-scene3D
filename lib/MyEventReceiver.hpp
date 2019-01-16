#pragma once

#ifndef MY_EVENT_RECEIVER_HPP
#define MY_EVENT_RECEIVER_HPP


#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <iostream>

#include <irrlicht.h>
#include "gui_ids.h"

#include "MyJoystickManager.hpp"


struct MyEventReceiver : irr::IEventReceiver
{


private:

  //attributs :
  irr::gui::IGUIEnvironment *gui;
  irr::scene::ISceneNode *node;
  irr::gui::IGUIWindow *window;
  bool isOpened = true; //items window state
  //methodes :

public:

  //constructeur(s) :
  MyEventReceiver();
  void set_gui(irr::gui::IGUIEnvironment *g);
  void set_node(irr::scene::ISceneNode *n);
  void set_window(irr::gui::IGUIWindow *w);

  //attributs :
  irr::scene::IAnimatedMeshSceneNode * perso1; //correspondra au perso principal; on pourra par la suite rajouter d'autres persos

  irr::scene::ICameraSceneNode * cam1; //correspondra a la camera principale; on pourra par la suite rajouter d'autres cams

  bool button_pressed;
  int  old_x, old_y;
  float speed = 3;




  MyJoystickManager joystick1;


  //methodes :
  bool OnEvent(const irr::SEvent &event); //surcharge de la fonction OnEvent() pour ajouter nos propres events

  bool keyboard(const irr::SEvent &event);

  bool mouse(const irr::SEvent &event);
  bool gui_manage(const irr::SEvent &event);

};
#endif
