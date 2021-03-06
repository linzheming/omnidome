/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_CANVAS_PLANAR_H_
#define OMNI_CANVAS_PLANAR_H_

#include <omni/visual/Plane.h>
#include <omni/canvas/Interface.h>

namespace omni
{
  namespace canvas
  {
    /// A planar canvas
    class Planar : public QObject, public Canvas
    {
        Q_OBJECT
        Q_INTERFACES(omni::canvas::Interface)
        Q_PLUGIN_METADATA(IID OMNI_CANVAS_INTERFACE_IID)
        OMNI_PLUGIN_INFO("Planar canvas","Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,Planar)

      Planar();
      ~Planar() final = default;

      void draw() const override;
      void drawAux() const override;

      void update() override;

      float width() const;
      void setWidth(float _width);

      float height() const;
      void setHeight(float _height);

      omni::Box bounds() const override;

      void fromPropertyMap(PropertyMap const&) override;
      void toPropertyMap(PropertyMap&) const override;

      ui::CanvasParameters* widget() override;

    private:
      float width_ = 1.0, height_ = 1.0;
      visual::Plane plane_;
    };
  }
}


#endif /* OMNI_CANVAS_PLANAR_H_ */
