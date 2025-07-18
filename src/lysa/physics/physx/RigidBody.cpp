/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#include <PxPhysicsAPI.h>
module lysa.nodes.rigid_body;

import lysa.application;
import lysa.constants;

namespace lysa {

    RigidBody::RigidBody(const std::shared_ptr<Shape>& shape,
                         const collision_layer layer,
                         const std::wstring& name):
        PhysicsBody(shape,
                    layer,
                    physx::PxActorType::eRIGID_DYNAMIC,
                    name,
                    RIGID_BODY) {
    }

    RigidBody::RigidBody(const std::wstring& name):
        PhysicsBody(0,
                    physx::PxActorType::eRIGID_DYNAMIC,
                    name,
                    RIGID_BODY) {
    }

    void RigidBody::createShape() {
        PhysicsBody::createShape();
        setMass(mass);
    }

    void RigidBody::createBody(const std::shared_ptr<Shape> &shape) {
        if (!actor) {
            PhysicsBody::createBody(shape);
            const auto body = static_cast<physx::PxRigidDynamic*>(actor);
            body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, gravityFactor == 0.0f);
            body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, getType() == KINEMATIC_BODY);
            setMass(mass);
        }
    }

    void RigidBody::setDensity(const float density) {
        this->density = density;
        if (!actor || !scene) return;
        const auto body = static_cast<physx::PxRigidDynamic*>(actor);
        physx::PxRigidBodyExt::updateMassAndInertia(*body, density);
    }

    void RigidBody::setVelocity(const float3& velocity) {
        if (!actor || !scene) return;
        const auto body = static_cast<physx::PxRigidDynamic*>(actor);
        if (all(velocity == FLOAT3ZERO)) {
            body->setLinearVelocity(physx::PxVec3(0, 0, 0));
        } else {
            const float3 v = mul(velocity, getRotationGlobal());
            body->setLinearVelocity(physx::PxVec3(v.x, v.y, v.z));
        }
    }

    float3 RigidBody::getVelocity() const {
        if (!actor || !scene) return FLOAT3ZERO;
        const auto v = static_cast<physx::PxRigidDynamic*>(actor)->getLinearVelocity();
        return float3{v.x, v.y, v.z};
    }

    void RigidBody::setGravityFactor(const float factor) {
        gravityFactor = factor;
        if (!actor || !scene) return;
        const auto body = static_cast<physx::PxRigidDynamic*>(actor);
        body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, gravityFactor == 0.0f);
    }

    void RigidBody::addForce(const float3& force) {
        if (!actor || !scene) return;
        static_cast<physx::PxRigidDynamic*>(actor)->addForce(
            physx::PxVec3(force.x, force.y, force.z),
            physx::PxForceMode::eFORCE);
        forceApplied = true;
    }

    void RigidBody::addForce(const float3& force, const float3&position) {
        if (!actor || !scene) return;
        const auto body = static_cast<physx::PxRigidDynamic*>(actor);
        physx::PxRigidBodyExt::addForceAtPos(
            *body,
            physx::PxVec3(force.x, force.y, force.z),
            physx::PxVec3(position.x, position.y, position.z),
            physx::PxForceMode::eFORCE);
        forceApplied = true;
    }

    void RigidBody::addImpulse(const float3& force) {
        if (!actor || !scene) return;
        static_cast<physx::PxRigidDynamic*>(actor)->addForce(
            physx::PxVec3(force.x, force.y, force.z),
            physx::PxForceMode::eIMPULSE);
    }

    void RigidBody::addImpulse(const float3& force, const float3&position) {
        if (!actor || !scene) return;
        const auto body = static_cast<physx::PxRigidDynamic*>(actor);
        physx::PxRigidBodyExt::addForceAtPos(
            *body,
            physx::PxVec3(force.x, force.y, force.z),
            physx::PxVec3(position.x, position.y, position.z),
            physx::PxForceMode::eIMPULSE);
    }

    void RigidBody::process(const float delta) {
        PhysicsBody::process(delta);
        if (forceApplied) {
            if (!actor || !scene) return;
            const auto body = static_cast<physx::PxRigidDynamic*>(actor);
            body->clearForce();
            forceApplied = false;
        }
    }

    void RigidBody::setMass(const float value) {
        mass = value;
        if (!actor || !scene) return;
        auto* body = static_cast<physx::PxRigidDynamic*>(actor);
        if (mass > 0.0f) {
            physx::PxRigidBodyExt::setMassAndUpdateInertia(*body, mass);
        } else {
            physx::PxRigidBodyExt::updateMassAndInertia(*body, density);
        }
    }

    float RigidBody::getMass() const {
        if (!actor || !scene) return mass;
        const auto* body = static_cast<physx::PxRigidDynamic*>(actor);
        return body->getMass();
    }

}
