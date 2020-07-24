#ifndef LIFE_ENGINE_LAYERSTAGING_HPP
#define LIFE_ENGINE_LAYERSTAGING_HPP
/**
 * This file is for staging development of various Engine Layers
 * When a layer is "done" you should make a dedicated file for it.
 * */
#include <layers/Layer.hpp>
#include <util/Time.h>
#include <kuma/openglmath>
#include <ecs/ecs>
#include <components/components>

#include "layers/staging/SystemStaging.hpp"
#include "layers/staging/ComponentStaging.hpp"
#include <bullet/btBulletDynamicsCommon.h>

namespace life {
	
	class BulletCommonRigidbodyBase {
	public:
		BulletCommonRigidbodyBase()
		 : m_collisionShapes{},
		   m_broadphase(nullptr),
		   m_dispatcher(nullptr),
		   m_solver(nullptr),
		   m_collisionConfiguration(nullptr),
		   m_dynamicsWorld(nullptr),
		   m_pickedBody(nullptr),
		   m_pickedConstraint(nullptr),
		   m_savedState(0),
		   m_oldPickingPos(),
		   m_hitPos(),
		   m_oldPickingDist()
		{}
		
	private:
		//keep the collision shapes, for deletion/cleanup
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
		btBroadphaseInterface* m_broadphase;
		btCollisionDispatcher* m_dispatcher;
		btConstraintSolver* m_solver;
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btDiscreteDynamicsWorld* m_dynamicsWorld;
		
		//data for picking objects
		class btRigidBody* m_pickedBody;
		class btTypedConstraint* m_pickedConstraint;
		int m_savedState;
		btVector3 m_oldPickingPos;
		btVector3 m_hitPos;
		btScalar m_oldPickingDist;
	};
	
    /// Important note about the reactphysics3d library: When the DynamicsWorld is
    /// destroyed, the rigidbodies in the world become invalid.
	class PhysicsLayer : public Layer {
    public:
        PhysicsLayer(std::shared_ptr<ecs::ECS>& world, const glm::vec3& default_gravity = {0,-9.81f,0})
         : Layer("PhysicsLayer"),
		   m_world{world},
		   m_physicsSystems{},
           m_gravity{default_gravity},
           m_collisionShapes{},
           m_collisionConfiguration{},
           // TODO: Test if the dtor of btCollisionDispatcher deletes the collisionConfiguration reference...
           m_dispatcher{&m_collisionConfiguration}, /// use the default collision dispatcher. For parallel processing you can use a different dispatcher (see Extras/BulletMultiThreaded)
           m_broadphase{},
           m_solver{}, // TODO: Use a different solver for multithreaded solutions
           m_dynamicsWorld{&m_dispatcher, &m_broadphase, &m_solver, &m_collisionConfiguration}
        {
			// m_collisionConfiguration.setConvexConvexMultipointIterations();
			// TODO: For proper use of the m_dynamicsWorld->GetDebugDrawer(), the camera matrix needs to be made available to the DebugDrawer.
			m_physicsSystems.PushSystem(PhysicsSyncSystem());
        }
		
		static bool OnBulletPhysicsCollision(btManifoldPoint& cp, 	const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
																	const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
			// TODO: What do I do with different Component types?
        	auto a = static_cast<BulletRigidBodyComponent*>(colObj0Wrap->getCollisionObject()->getUserPointer());
			auto b = static_cast<BulletRigidBodyComponent*>(colObj1Wrap->getCollisionObject()->getUserPointer());
			
        	return false;
		}
        
        void OnAttach() override {
        	// Set the bullet physics collision callback
        	gContactAddedCallback = OnBulletPhysicsCollision;
        	/// Note: Other bulletphysics callbacks:
        	// gContactBreakingThreshold
        	// gContactDestroyedCallback
        	// gContactEndedCallback
        	// gContactProcessedCallback
        	// gContactStartedCallback
        	
        	// Initialize the physics world
        	m_dynamicsWorld.setGravity(convert_vec3(m_gravity));
        	
        	// CreateTestScene();
        }

        void OnDetach() override {
        	// TODO: Delete the world properly
        }
        
        void OnUpdate() override {
        	m_dynamicsWorld.stepSimulation(Time::GetFrameTime());
        	// Update the ECS systems associated with physics
        	m_world->UpdateSystems(m_physicsSystems);
        }
        
        void CreateTestScene() {
        	// TODO: Decide where code such as this should be at.
        	auto sz = btVector3(btScalar(50.),btScalar(50.),btScalar(50.));
			auto smallsize = btVector3(btScalar(.1),btScalar(.1),btScalar(.1));
			btBoxShape* ground = new btBoxShape(sz);
        	m_collisionShapes.push_back(ground);
			btTransform groundTransform{};
			groundTransform.setIdentity();
			groundTransform.setOrigin(btVector3(0, -50, 0));
			{
				auto* rbdy = createRigidBody({0.}, groundTransform, ground, btVector4(0, 0, 1, 1));
				/// Create an entity of a floor, that is 50x50x50 at (0,-50, 0)
				/// Probably an entity of type E<Material, StaticMesh, Transform, BulletRigidBody>
				{
					MaterialComponent m{};
					TransformComponent t{kuma::Transform({0,-50,0},{0.0f, 0.0f, 0.0f, 1.0f},{sz.getX(),sz.getY(),sz.getZ()})};
					StaticMeshComponent sm{m.material, t.transform, "./res/wal67ar_small.jpg"};
					BulletRigidBodyComponent r{rbdy, t.transform};
					m_world->makeEntity(std::move(m), std::move(sm), std::move(t), std::move(r));
				}
			}
			{
				//create a few dynamic rigidbodies
				// Re-using the same collision is better for memory usage and performance
				btBoxShape* colShape = new btBoxShape(smallsize);
		
				//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
				m_collisionShapes.push_back(colShape);
		
				/// Create Dynamic Objects
				btTransform startTransform;
				startTransform.setIdentity();
		
				btScalar mass(4.0f);
				
				//rigidbody is dynamic if and only if mass is non zero, otherwise static
				bool isDynamic = (mass != 0.f);
		
				btVector3 localInertia(0, 0, 0);
				if (isDynamic)
					colShape->calculateLocalInertia(mass, localInertia);
		
				for (int k = 0; k < 5; k++) {
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 5; j++) {
							startTransform.setOrigin(btVector3(
									btScalar(0.2 * i),
									btScalar(2 + .2 * k),
									btScalar(0.2 * j)));
							
							auto* rbdy = createRigidBody(mass, startTransform, colShape);
							/// Probably an entity of type E<Material, StaticMesh, Transform, BulletRigidBody>
							{
								MaterialComponent m{};
								TransformComponent t{kuma::Transform({0.2*i,2+.2*k,0.2*j},{0.0f, 0.0f, 0.0f, 1.0f},{smallsize.getX(),smallsize.getY(),smallsize.getZ()})};
								StaticMeshComponent sm{m.material, t.transform};
								BulletRigidBodyComponent r{rbdy, t.transform};
								m_world->makeEntity(std::move(m), std::move(sm), std::move(t), std::move(r));
							}
						}
					}
				}
			}
        }
		
		btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color = btVector4(1, 0, 0, 1)) {
			LIFE_ASSERT((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE), "Physics shape is invalid.");
			
			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				shape->calculateLocalInertia(mass, localInertia);
			
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			
			btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
			
			btRigidBody* body = new btRigidBody(cInfo);
			//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
			btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
		body->setWorldTransform(startTransform);
#endif  //
			
			// body->setUserIndex(-1);
			// Collision Callback flag set
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			
			m_dynamicsWorld.addRigidBody(body);
			return body;
		}

	private:
		inline btVector3 convert_vec3(const glm::vec3& v) { return {v.x,v.y,v.z}; }
		
		std::shared_ptr<ecs::ECS> m_world;
        ecs::SystemList m_physicsSystems;
        
        glm::vec3 m_gravity;
        btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
		btDefaultCollisionConfiguration m_collisionConfiguration;
		btCollisionDispatcher m_dispatcher;
		btDbvtBroadphase m_broadphase;
		btSequentialImpulseConstraintSolver m_solver;
		btDiscreteDynamicsWorld m_dynamicsWorld;
    };

}

#endif //LIFE_ENGINE_LAYERSTAGING_HPP
