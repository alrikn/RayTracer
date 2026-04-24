#include "SpecularLight.hpp"
#include "Ray.hpp"
#include "Vector3d.hpp"
#include <cmath>

namespace RayTracer {

  SpecularLight::SpecularLight(const Math::Vector3d &direction, double intensity,
                                const Math::Point3d &camera_pos, double shininess,
                                double specular_strength, const Math::Vector3d &color)
  {
      this->_direction = direction;
      this->_direction.normalize();
      this->_intensity = intensity;
      this->_camera_pos = camera_pos;
      this->_shininess = shininess;
      this->_specular_strength = specular_strength;
      this->_color = color;
  }

  Math::Vector3d SpecularLight::intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const
  {
    const double epsilon = 1e-4;

    if (_specular_strength <= 0.0 || _shininess <= 0.0)
        return Math::Vector3d(0, 0, 0);

    Math::Vector3d light_dir = _direction * -1.0;
    light_dir.normalize();

    if (hit.normal.dot(light_dir) <= 0.0)
        return Math::Vector3d(0, 0, 0);

    RayTracer::Ray shadow_ray(hit.point + hit.normal * epsilon, light_dir);

    for (const auto& object : objects) {
        auto shadow_hit = object->hits(shadow_ray);
        if (shadow_hit.has_value() && shadow_hit->distance > epsilon)
            return Math::Vector3d(0, 0, 0);
    }

    Math::Vector3d view_dir(
        _camera_pos.x - hit.point.x,
        _camera_pos.y - hit.point.y,
        _camera_pos.z - hit.point.z
    );
    view_dir.normalize();

    Math::Vector3d reflect_dir = light_dir - hit.normal * 2.0 * hit.normal.dot(light_dir);
    reflect_dir.normalize();

    double spec_angle = std::max(reflect_dir.dot(view_dir), 0.0);
    double specular = std::pow(spec_angle, _shininess) * _specular_strength;

    return _color * _intensity * specular;
  }

}
