#version 430
precision highp float;
layout(local_size_x = 4, local_size_y = 4) in;
layout(rgba32f, binding = 0) uniform image2D renderedTexture;
uniform vec3 camera_dir;
uniform vec3 camera_up;
uniform vec3 camera_right;
uniform vec3 camera_pos;

struct HitInfo{
    bool hit;
    float dist_from_cam;
};
struct Square{
    vec3 pos, norm, uDir, vDir, color;
    float length;
};
struct Sphere{
    vec3 center, color;
    float radius2, inv_radius;
};
struct Ray{
    vec3 origin, direction;
};
struct Cylinder{
    vec3 pos, axis, color;
    float radius, length;
};

uniform Square squares[3];
uniform Sphere spheres[100];
uniform Cylinder cylinders[100];
uniform vec3 light;
uniform float cosPhi[24];
uniform float sinPhi[24];
uniform float time;
uniform int numCylinders, numSpheres;

#define MAX pow(10.0, 6.0)
const float width = 1024.0;
const float height = 768.0;
const float screen_dist = 1.0 * width;
const float light_radius = 200.0;
const float light_radius2 = 40000.0;

HitInfo shoot_ray_at_cylinder(Ray ray, Cylinder cylinder, float closest_dist){
    bool hit = false;
    float min_dist = 0.0;
    vec3 Dir = cylinder.pos - ray.origin;
    float RayCyl = dot(ray.direction, cylinder.axis);
    float oneMinRayCyl2 = 1.0 - RayCyl * RayCyl;
    float DirRay = dot(Dir, ray.direction);
    float DirCyl = dot(Dir, cylinder.axis);
    float DirDir = dot(Dir, Dir);
    float dist2 = 2.0 * DirRay * DirCyl * RayCyl - DirCyl * DirCyl
                - DirRay * DirRay + oneMinRayCyl2 * DirDir;
    dist2 = dist2 / oneMinRayCyl2;
    if( dist2 < cylinder.radius * cylinder.radius){
        float origin_to_closest = (DirRay - RayCyl * DirCyl)/oneMinRayCyl2;
        min_dist = origin_to_closest - sqrt((cylinder.radius * cylinder.radius - dist2)/oneMinRayCyl2);
        if( min_dist > 0.0 && min_dist < closest_dist){
            vec3 cylinder_to_intersect = min_dist * ray.direction - Dir;
            float dist_from_cylinder_pos2 = dot(cylinder.axis, cylinder_to_intersect);
            if(dist_from_cylinder_pos2 < cylinder.length && dist_from_cylinder_pos2 >= 0.0){
                hit = true;
            }
        }
    }
    return HitInfo(hit, min_dist);
}

HitInfo shoot_ray_at_sphere(Ray ray, Sphere sphere, float closest_dist){
    bool hit = false;
    float dist_from_cam = 0.0;
    float dist_origin_to_closest = dot(ray.direction, sphere.center - ray.origin);
    if(dist_origin_to_closest > 0.0 && dist_origin_to_closest < closest_dist){
        vec3 closest_to_center = ray.origin + dist_origin_to_closest * ray.direction - sphere.center;
        float dist_closest_to_center2 = dot(closest_to_center, closest_to_center);
        if(dist_closest_to_center2 <= sphere.radius2){
            dist_from_cam = dist_origin_to_closest - sqrt(sphere.radius2 - dist_closest_to_center2);
            if(dist_from_cam < closest_dist){
                hit = true;
            }
        }
    }
    return HitInfo(hit, dist_from_cam);
}

HitInfo shoot_ray_at_square(Ray ray, Square square, float closest_dist){
    float cos_ray_normal = dot(ray.direction, square.norm);
    vec3 orig_to_square = square.pos - ray.origin;
    float rho = dot(orig_to_square, square.norm) / cos_ray_normal;
    bool hit = false;
    if(rho >= 0.0 && rho < closest_dist){
        float sigma = -dot(cross(orig_to_square, square.vDir), ray.direction) / cos_ray_normal;
        float rest = mod(sigma, 600.0);
        if(sigma >= 0.0 && sigma <= square.length && rest >= 10.0){
            float theta = dot(cross(orig_to_square, square.uDir), ray.direction) / cos_ray_normal;
            rest = mod(theta, 600.0);
            if(theta >= 0.0 && theta <= square.length && rest >= 10.0){
                hit = true;
            }
        }
    }    
    return HitInfo(hit, rho);
}

void main() {
  ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  vec3 ray_dir = normalize(screen_dist * camera_dir 
                          + (float(pixel_coords.x) - (width / 2.0)) * camera_right 
                          + (float(pixel_coords.y) - (height / 2.0)) * camera_up);
  vec3 color = vec3(0.0, 0.0, 0.0);
  Ray ray = Ray(camera_pos, ray_dir);
  float weight = 1.0;

  for(int bounces = 0; bounces < 3; bounces++){
      int hitObjectId = 0;
      HitInfo bestHit = HitInfo(false, MAX);
      for(int j = 0; j < 3; j++){
          HitInfo thisHit = shoot_ray_at_square(ray, squares[j], bestHit.dist_from_cam);
          if(thisHit.hit){    
              bestHit = thisHit;
              hitObjectId = j;
          }
      }

      for(int j = 0; j < 50; j++){
          if( j >= numSpheres){
              break;
          }
          else{
              HitInfo thisHit = shoot_ray_at_sphere(ray, spheres[j], bestHit.dist_from_cam);
              if(thisHit.hit){
                  bestHit = thisHit;
                  hitObjectId = 3 + j;
              }
          }
      }

      for(int j = 0; j < 50; j++){
          if( j >= numCylinders){
              break;
          }
          else{
              HitInfo thisHit = shoot_ray_at_cylinder(ray, cylinders[j], bestHit.dist_from_cam);
              if(thisHit.hit){
                  bestHit = thisHit;
                  hitObjectId = 3 + numSpheres + j;
              }
          }
      }

      if(bestHit.hit){
          float diff_coeff = 0.0;
          vec3 normal_at_point, hit_color;
          vec3 intersect_point = ray.origin + 0.99999 * bestHit.dist_from_cam * ray.direction;
          if ( hitObjectId < 3){
              normal_at_point = squares[hitObjectId].norm;
              hit_color = squares[hitObjectId].color;
          }
          else if( hitObjectId < 3 + numSpheres){
              normal_at_point = spheres[hitObjectId - 3].inv_radius * (ray.origin + bestHit.dist_from_cam * ray.direction - spheres[hitObjectId - 3].center);
              hit_color = spheres[hitObjectId - 3].color;
          }
          else{
              int hitCylinderId = hitObjectId - 3 - numSpheres;
              vec3 cylinder_pos_to_intersect = ray.origin + bestHit.dist_from_cam * ray.direction - cylinders[hitCylinderId].pos;
              normal_at_point = normalize(cylinder_pos_to_intersect - dot(cylinders[hitCylinderId].axis, cylinder_pos_to_intersect) * cylinders[hitCylinderId].axis);
              hit_color = cylinders[hitCylinderId].color;
          }
          vec3 point_to_light = light - intersect_point;
          float dist_from_light2 = dot(point_to_light, point_to_light);
          vec3 shadow_ray = point_to_light / sqrt(dist_from_light2);

          bool hit_light = true;
          float light_cos = dot(normal_at_point, shadow_ray);
          Ray shadowRay = Ray(intersect_point, shadow_ray);
          if(light_cos > 0.0){
              for (int j = 0; j < 3; j++){
                  if(shoot_ray_at_square( shadowRay, squares[j], MAX ).hit){
                      hit_light = false;
                      break;
                  }
              }

              if(hit_light){
                  for (int j = 0; j < 50; j++){
                      if( j >= numSpheres){
                          break;
                      }
                      else{
                          if(shoot_ray_at_sphere( shadowRay, spheres[j], MAX ).hit){
                              hit_light = false;
                              break;
                          }
                      }
                  }
              }

              if(hit_light){
                  for (int j = 0; j < 50; j++){
                      if( j >= numCylinders){
                          break;
                      }
                      else{
                          if(shoot_ray_at_cylinder( shadowRay, cylinders[j], MAX ).hit){
                              hit_light = false;
                              break;
                          }
                      }
                  }
              }

              if(hit_light){
                  diff_coeff = light_cos;
              }
          }
          
          ray = Ray(intersect_point, reflect(ray.direction, normal_at_point));
          float spec_coeff = pow(max(0.0, dot(shadow_ray, ray.direction)), 40.0);
          diff_coeff = max(0.3, diff_coeff);
          vec3 light_color = vec3(1.0, 1.0, 1.0);
          hit_color = vec3(hit_color.x * light_color.x, hit_color.y * light_color.y, hit_color.z * light_color.z);
          color += weight * diff_coeff * (hit_color + spec_coeff * light_color);
          weight *= 0.5 * diff_coeff;
      }
      else{
          if(bounces == 0){ color = vec3(0.7,0.7,0.7); }
          break;
      }
                          
      if(weight < 0.001){
          break;
      }
  }
  imageStore(renderedTexture, pixel_coords, vec4(color, 1.0));
}