#include "inputs.h"
#include <stdexcept>

extern "C" {
  #include <xdo.h>
}

void Inputs::XdoDeleter::operator()(xdo_t* xdo) const {
  if (xdo) xdo_free(xdo);
}

Inputs::Inputs(const std::string& display) {
  xdo_.reset(xdo_new(display.empty() ? nullptr : display.c_str()));
  if (!xdo_) {
    throw std::runtime_error("Failed to connect to X display: " + display);
  }
}

void Inputs::mouse(int x, int y, int button) {
  xdo_move_mouse(xdo_.get(), x, y, CURRENTWINDOW);
  xdo_mouse_down(xdo_.get(), CURRENTWINDOW, button);
  xdo_mouse_up(xdo_.get(), CURRENTWINDOW, button);
}

void Inputs::mouseMove(int x, int y) {
  xdo_move_mouse(xdo_.get(), x, y, CURRENTWINDOW);
}

void Inputs::mouseDrag(int x, int y) {
  xdo_move_mouse(xdo_.get(), x, y, CURRENTWINDOW);
    xdo_mouse_down(xdo_.get(), CURRENTWINDOW, 1);
    xdo_move_mouse(xdo_.get(), 497, 260, CURRENTWINDOW);
  xdo_mouse_up(xdo_.get(), CURRENTWINDOW, 1);
}

std::tuple<int, int> Inputs::mouseLocation() {
  int X, Y, SCREEN;
  xdo_get_mouse_location(xdo_.get(), &X, &Y, &SCREEN);
  return std::make_tuple(X, Y);
}

void Inputs::keyboard(std::string key) {
  xdo_send_keysequence_window_down(xdo_.get(), CURRENTWINDOW, key.c_str(), 0);
  xdo_send_keysequence_window_up(xdo_.get(), CURRENTWINDOW, key.c_str(), 0);
}

void Inputs::keywith(const char* key) {
  xdo_send_keysequence_window(xdo_.get(), CURRENTWINDOW, key, 12000);
}

void Inputs::shiftDown() {
  xdo_send_keysequence_window_down(xdo_.get(), CURRENTWINDOW, "shift", 0);
}

void Inputs::shiftUp() {
  xdo_send_keysequence_window_up(xdo_.get(), CURRENTWINDOW, "shift", 0);
}