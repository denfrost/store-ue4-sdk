// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#include "XsollaStoreImageLoader.h"

#include "XsollaStoreDefines.h"

#include "Framework/Application/SlateApplication.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FXsollaLoginModule"

UXsollaStoreImageLoader::UXsollaStoreImageLoader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UXsollaStoreImageLoader::LoadImage(FString URL, const FOnImageLoaded& SuccessCallback)
{
	UE_LOG(LogXsollaStore, Verbose, TEXT("%s: Loading imager from: %s"), *VA_FUNC_LINE, *URL);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UXsollaStoreImageLoader::LoadImage_HttpRequestComplete, SuccessCallback);
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb(TEXT("GET"));

	HttpRequest->ProcessRequest();
}

void UXsollaStoreImageLoader::LoadImage_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FOnImageLoaded SuccessCallback)
{
	if (bSucceeded && HttpResponse.IsValid())
	{
		const TArray<uint8>& ImageData = HttpResponse->GetContent();

		TSharedPtr<FSlateDynamicImageBrush> ImageBrush;

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		const EImageFormat ImageType = ImageWrapperModule.DetectImageFormat(ImageData.GetData(), ImageData.Num());
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageType);

		if (!ImageWrapper.IsValid())
		{
			UE_LOG(LogXsollaStore, Error, TEXT("%s: Invalid image wrapper"), *VA_FUNC_LINE);
		}
		else if (ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
		{
			const int32 BytesPerPixel = ImageWrapper->GetBitDepth();
			const TArray<uint8>* RawData = nullptr;

			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, BytesPerPixel, RawData) && RawData && RawData->Num() > 0)
			{
				const FName ResourceName = FName(*FString::Printf(TEXT("XsollaStoreImage_%s"), *FGuid::NewGuid().ToString()));

				if (FSlateApplication::Get().GetRenderer()->GenerateDynamicImageResource(ResourceName, ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), *RawData))
				{
					SuccessCallback.ExecuteIfBound(FSlateDynamicImageBrush(ResourceName, FVector2D(ImageWrapper->GetWidth(), ImageWrapper->GetHeight())));
				}
				else
				{
					UE_LOG(LogXsollaStore, Error, TEXT("%s: Can't generate resource"), *VA_FUNC_LINE);
				}
			}
			else
			{
				UE_LOG(LogXsollaStore, Error, TEXT("%s: Can't get raw data"), *VA_FUNC_LINE);
			}
		}
		else
		{
			UE_LOG(LogXsollaStore, Error, TEXT("%s: Can't load compressed data"), *VA_FUNC_LINE);
		}
	}
	else
	{
		UE_LOG(LogXsollaStore, Error, TEXT("%s: Failed to download image"), *VA_FUNC_LINE);
	}
}

#undef LOCTEXT_NAMESPACE