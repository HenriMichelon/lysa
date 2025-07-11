/*
 * Copyright (c) 2024-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#ifdef PHYSIC_ENGINE_JOLT
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#endif
#ifdef PHYSIC_ENGINE_PHYSX
#include <PxPhysicsAPI.h>
#endif
export module lysa.nodes.character;

import lysa.constants;
import lysa.signal;
import lysa.viewport;
import lysa.nodes.collision_object;
import lysa.resources.shape;

export namespace lysa {

    /**
     * %A 3D physics body specialized for characters moved by code
     */
    class Character : public CollisionObject
#ifdef PHYSIC_ENGINE_JOLT
                    ,
                    public JPH::BroadPhaseLayerFilter,
                    public JPH::BodyFilter,
                    public JPH::CharacterContactListener
#endif
#ifdef PHYSIC_ENGINE_PHYSX
        ,
        public physx::PxUserControllerHitReport,
        public physx::PxQueryFilterCallback
#endif
    {
    public:
        /**
         * Signal called whenever the character collides with a body and reports the first contact point in a CollisionObject::Collision<br>
         */
        static inline const Signal::signal on_collision = "on_character_collision";

        /**
         * Creates a Character with a given collision `shape`,
         * belonging to the `layer` layers and detecting collisions
         * with bodies having a layer in the `mask` value.
         * @param height Height of the collision shape capsule shape
         * @param radius Radius of the collision shape capsule
         * @param layer Collision layer
         * @param name Name of the node
         */
        explicit Character(float height,
                           float radius,
                           collision_layer layer,
                           const std::wstring &name = TypeNames[CHARACTER]);

        /**
         * Sets a new capsule shape, recreates the virtualCharacter in the physic system
         */
        void setShape(float height, float radius);


        /**
         * Returns `true` if the Character is on a ground
         */
        bool isOnGround() const;

        /**
         * Returns `true` if `object` is the ground
         */
        bool isGround(const CollisionObject &object) const;

        /**
         * Returns the velocity in the world space of the ground.
         */
        float3 getGroundVelocity() const;

        /**
         * Returns the ground node, if any
         */
        Node* getGround() const;

        /**
         * Returns the UP axis for this Character
         */
        const auto& getUp() const { return upVector; }

        /**
         * Sets the UP axis for this Character
         */
        void setUp(const float3& vector);

        /**
        * Returns the list of the currently colliding bodies
        */
        std::list<Collision> getCollisions() const;

        /**
         * Moves the virtualCharacter using this velocity
         */
        void setVelocity(const float3& velocity);

        /**
         * Set the maximum angle of slope that character can still walk on (degrres)
         */
        void setMaxSlopeAngle(float angle);

        /**
         * Returns the height of the capsule collision shape
         */
        float getHeight() const { return height; }

        /**
         * Returns the radius of the capsule collision shape
         */
        float getRadius() const { return radius; }

        /**
         * Sets the current visibility of the character.
         */
        void setVisible(bool visible = true) override;

        /**
         * Sets the current collision layer
         */
        void setCollisionLayer(collision_layer layer) override;

        /**
         * Returns the linear velocity of the character
         */
        float3 getVelocity() const;

        ~Character() override = default;

    protected:
        float height;
        float radius;
        float3 upVector{AXIS_UP};
        float yDelta{0};

        void setPositionAndRotation() override;

        void physicsProcess(float delta) override;

        void process(float alpha) override;

        void attachToViewport(Viewport* viewport) override;

        void enterScene() override;

        void resume() override;

#ifdef PHYSIC_ENGINE_JOLT
        std::unique_ptr<JPH::CharacterVirtual> virtualCharacter;
        std::unique_ptr<JPH::ObjectLayerFilter> objectLayerFilter;
        void OnContactAdded(const JPH::CharacterVirtual *  inCharacter,
                            const JPH::BodyID &            inBodyID2,
                            const JPH::SubShapeID &        inSubShapeID2,
                            JPH::RVec3Arg                  inContactPosition,
                            JPH::Vec3Arg                   inContactNormal,
                            JPH::CharacterContactSettings &ioSettings) override;
        bool OnContactValidate(const JPH::CharacterVirtual *  inCharacter,
                            const JPH::BodyID &            inBodyID2,
                            const JPH::SubShapeID &        inSubShapeID2) override;
        bool ShouldCollide(const JPH::BroadPhaseLayer inLayer) const override { return true; }
        bool ShouldCollide(const JPH::BodyID &inBodyID) const override;
        bool ShouldCollideLocked(const JPH::Body &inBody) const override;
#endif
#ifdef PHYSIC_ENGINE_PHYSX
        bool onGround{false};
        CollisionObject* ground{nullptr};
        std::list<Collision> currentContacts;
        float3 velocity{0.0f};
        float maxSlopeAngle{45.0f};
        physx::PxCapsuleController* capsuleController{nullptr};
        physx::PxCapsuleGeometry capsule;
        void onShapeHit(const physx::PxControllerShapeHit &hit) override;
        void onControllerHit(const physx::PxControllersHit &hit) override;
        void onObstacleHit(const physx::PxControllerObstacleHit &hit) override;
        physx::PxQueryHitType::Enum preFilter(
            const physx::PxFilterData &filterData,
            const physx::PxShape *shape,
            const physx::PxRigidActor *actor,
            physx::PxHitFlags &queryFlags
        ) override;
        physx::PxQueryHitType::Enum postFilter(
        const physx::PxFilterData &filterData,
        const physx::PxQueryHit &hit,
        const physx::PxShape *shape,
        const physx::PxRigidActor *actor
        ) override;
#endif
    };

}
