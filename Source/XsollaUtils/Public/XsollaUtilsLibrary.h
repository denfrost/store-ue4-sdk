// Copyright 2020 Xsolla Inc. All Rights Reserved.

#pragma once

#include "Components/CheckBox.h"
#include "EngineMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XsollaUtilsDataModel.h"
#include "XsollaUtilsLibrary.generated.h"

class UXsollaUtilsImageLoader;
class FJsonObject;

UCLASS()
class XSOLLAUTILS_API UXsollaUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	static void Internal_AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject,
		FXsollaParameters CustomParameters, const FString& FieldName = "");

public:
	/** Direct access to the image loader object. */
	UFUNCTION(BlueprintPure, Category = "Xsolla|Utils")
	static UXsollaUtilsImageLoader* GetImageLoader();

	/** Make FDateTime structure based on a given timestamp. */
	UFUNCTION(BlueprintPure, Category = "Xsolla|Utils")
	static FDateTime MakeDateTimeFromTimestamp(int64 Time);

	/** Get seconds from DateTime to UnixTimestamp. */
	UFUNCTION(BlueprintPure, Category = "Xsolla|Utils")
	static int64 GetSecondsFromUnixTimestamp(FDateTime DateTime);

	template <typename TEnum>
	static FString GetEnumValueAsString(const FString& EnumName, TEnum Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!EnumPtr)
		{
			return FString("Invalid");
		}
		const FString ValueStr = EnumPtr->GetNameByValue(static_cast<int64>(Value)).ToString();
		return ValueStr.Replace(*FString::Printf(TEXT("%s::"), *EnumName), TEXT(""));
	}

	template <typename EnumType>
	static EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
	{
		UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!Enum)
		{
			return EnumType(0);
		}
		return static_cast<EnumType>(Enum->GetValueByName(FName(*String)));
	}

	/** Add parameters to json object root */
	static void AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject, FXsollaParameters CustomParameters);

	/** Add parameters to json object by FieldName */
	static void AddParametersToJsonObjectByFieldName(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, FXsollaParameters CustomParameters);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (int)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_IntToXsollaJsonVariant(int Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (float)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_FloatToXsollaJsonVariant(float Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (string)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_StringToXsollaJsonVariant(const FString& Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (bool)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_BoolToXsollaJsonVariant(bool Value);
};
