// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FMocopiLiveLinkModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OnCultureChanged();

private:

  FDelegateHandle mCultureChangedHandle;

  void SetPluginLocalization(FString ISOLanguageCode);

};
