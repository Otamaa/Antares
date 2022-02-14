#pragma once

#include <BulletClass.h>

#include "../_Container.hpp"
#include "../../Ares.h"

#include "../../Misc/Debug.h"
#include <ParticleSystemClass.h>
#include <Utilities/Constructs.h>

class SuperWeaponTypeClass;

class BulletExt
{
public:
	using base_type = BulletClass;

	class ExtData final : public Extension<BulletClass>
	{
	public:
		SuperWeaponTypeClass *NukeSW;
		UniqueGamePtr<ParticleSystemClass> ParticleSys;
		ExtData(BulletClass* OwnerObject) : Extension(OwnerObject),
			NukeSW(nullptr)
			, ParticleSys()
		{ }

		virtual ~ExtData() = default;

		bool DamageOccupants();
		void CreateParticleSys();

		virtual void InvalidatePointer(void *ptr, bool bRemoved) override 
		{
			if (ParticleSys.get() && (ParticleSys.get() == ptr))
					ParticleSys.release();	
		}

		virtual void LoadFromStream(AresStreamReader &Stm) override;

		virtual void SaveToStream(AresStreamWriter &Stm) override;

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<BulletExt> {
	public:
		ExtContainer();
		~ExtContainer();
	};

	static ExtContainer ExtMap;


};
