#ifndef INCLUDED_SPECULARLIGHT_HPP
    #define INCLUDED_SPECULARLIGHT_HPP

#include <iostream>
#include "ILight.hpp"
#include "Vector3d.hpp"


namespace RayTracer {

  class SpecularLight : public ILight
  {
      private:
          Math::Vector3d _direction;
          double _intensity = 1.0;
          Math::Vector3d _color = Math::Vector3d(255, 255, 255);
      public:
          SpecularLight(const Math::Vector3d &direction, double intensity = 1.0, const Math::Vector3d &color = Math::Vector3d(255, 255, 255));
          ~SpecularLight() = default;

          Math::Vector3d intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const override;
  };
  }
#endif
