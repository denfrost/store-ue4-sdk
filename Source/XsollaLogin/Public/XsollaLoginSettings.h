// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#pragma once

#include "XsollaLoginDefines.h"

#include "Blueprint/UserWidget.h"

#include "XsollaLoginSettings.generated.h"

/** You can store user data on the Xsolla side, which is the default option, or in your own storage. */
UENUM(BlueprintType)
enum class EUserDataStorage : uint8
{
	/** User data is stored on the Xsolla side. */
	Xsolla UMETA(DisplayName = "Xsolla storage"),

	/** If the user data is stored on your side, proxy requests are used. */
	Custom UMETA(DisplayName = "Custom storage"),
};

/** Target platform name. */
UENUM(BlueprintType)
enum class EXsollaTargetPlatform : uint8
{
	PlaystationNetwork,
	XboxLive,
	Xsolla,
	PcStandalone,
	NintendoShop,
	GooglePlay,
	AppStoreIos,
	AndroidStandalone,
	IosStandalone,
	AndroidOther,
	IosOther,
	PcOther
};

UCLASS(config = Engine, defaultconfig)
class XSOLLALOGIN_API UXsollaLoginSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/** Project ID from your Publisher Account. Required. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	FString ProjectID;

	/** Login ID in the UUID format from your Publisher Account. Required. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	FString LoginID;

	/**
	 * URL to redirect the user to after registration, authentication, or password reset.
	 * Must be identical to a Callback URL specified in Publisher Account in Login settings.
	 * Required if there are several Callback URLs.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	FString CallbackURL;

	/** API methods will call different URLs depending on the selected storage method. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	EUserDataStorage UserDataStorage;

	/** Custom class to handle authentication via a social network. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	TSubclassOf<UUserWidget> OverrideBrowserWidgetClass;

	/** If enabled, the Login SDK will deactivate the existing user JWT values and activate the one generated during the last successful authentication. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Invalidate Existing Sessions", Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	bool InvalidateExistingSessions;

	/** If enabled, the Login SDK will use OAuth 2.0 protocol in order to authorize the user. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Use OAuth2", Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	bool UseOAuth2;

	/** Your application ID. You will get it after sending a request to enable the OAuth 2.0 protocol. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "UseOAuth2 && !bCustomAuthViaAccessToken"))
	FString ClientID;

	/** If enabled, the Login SDK will imitate platform-specific authentication so you can try account linking from different platforms. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Use Cross-Platform Account Linking", Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	bool UseCrossPlatformAccountLinking;

	/**
	 * URL used to link the user platform account to the main account with a generated code.
	 * The main account is the Xsolla Login project which other Xsolla Login projects (platform accounts) are linked to.
	 * Main and platform accounts are created in Publisher Account.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "UseCrossPlatformAccountLinking && !bCustomAuthViaAccessToken"))
	FString AccountLinkingURL;

	/** URL used for a target platform user account authentication. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "UseCrossPlatformAccountLinking && !bCustomAuthViaAccessToken"))
	FString PlatformAuthenticationURL;

	/** Target platform for cross-platform account linking. If using Xsolla Store, make sure that in the Store settings the same platform is chosen. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "UseCrossPlatformAccountLinking && !bCustomAuthViaAccessToken"))
	EXsollaTargetPlatform Platform;

	/** Unique identifier of a target platform user account. You can enter any alphanumeric combination. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings",
		meta = (EditCondition = "UseCrossPlatformAccountLinking && Platform != EXsollaTargetPlatform::Xsolla && !bCustomAuthViaAccessToken"))
	FString PlatformAccountID;

	/** Flag indicating whether Xsolla cached credentials should be encrypted and decrypted using the XsollaSaveEncryptionKey secondary encryption key. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	bool EncryptCachedCredentials;

	/** AES-256 encryption key used for cached credentials encryption. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Settings", meta = (EditCondition = "EncryptCachedCredentials && !bCustomAuthViaAccessToken"))
	FString XsollaSaveEncryptionKey;

	/** Demo Project ID. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Demo", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	FString DemoProjectID;

	/** Demo Login ID. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Demo", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	FString DemoLoginID;

	/** Request user nickname after successful authorization in case one is missing. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Demo", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	bool RequestNickname;

	/** If enabled, the Login SDK will use Steam as default authorization mechanism. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Demo", meta = (EditCondition = "!bCustomAuthViaAccessToken"))
	bool bUseSteamAuthorization;

	/** If enabled, the Inventory SDK will use auth via the access token. */ 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Custom Auth")
	bool bCustomAuthViaAccessToken;

	/** URL for auth via the access token.  */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Xsolla Login Custom Auth", meta = (EditCondition = "bCustomAuthViaAccessToken"))
	FString CustomAuthServerURL;
};
