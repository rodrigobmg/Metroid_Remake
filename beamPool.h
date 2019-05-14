#include <list>
#include "beam.h"
#include "collisionStrategy.h"

class BeamPool {
public:
  BeamPool(const std::string&);
  BeamPool(const BeamPool&);
  ~BeamPool();
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& vel);

  unsigned int beamCount() const { return beamList.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  bool shooting() const { return beamList.empty(); }
  bool collidedWith(const Drawable* obj) const;
private:
  std::string name;
  CollisionStrategy* strategy;
  float frameInterval;
  float timeSinceLastFrame;
  mutable std::list<Beam> beamList; // BeamPool is a list of Beam
  mutable std::list<Beam> freeList;   // when a Beam is out of range

  BeamPool& operator=(const BeamPool&);   // (Item 6)
};
