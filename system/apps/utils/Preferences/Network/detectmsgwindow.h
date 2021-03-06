// Network Preferences :: (C)opyright Daryl Dudey 2002
//
// This is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef __DETECTMSGWINDOW_H_
#define __DETECTMSGWINDOW_H_

#include <gui/window.h>
#include <gui/layoutview.h>

class DetectMsgWindow : public os::Window {
public:
  DetectMsgWindow(const os::Rect& cFrame);
  ~DetectMsgWindow();
  virtual void HandleMessage(os::Message* pcMessage);

  bool OkToQuit(void);

private:
  os::LayoutView *pcLRoot;
  os::VLayoutNode *pcVLRoot;
};

#endif /* __DETECTMSGWINDOW_H_ */







