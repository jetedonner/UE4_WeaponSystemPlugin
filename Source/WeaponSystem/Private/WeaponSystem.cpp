//
//  WeaponSystem.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem.h"

#define LOCTEXT_NAMESPACE "FWeaponSystemModule"
DEFINE_LOG_CATEGORY(WeaponSysPlugin);

void FWeaponSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FWeaponSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWeaponSystemModule, WeaponSystem)
