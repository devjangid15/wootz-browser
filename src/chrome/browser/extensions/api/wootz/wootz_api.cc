// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/extensions/api/wootz/wootz_api.h"

#include <stddef.h>

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/lazy_instance.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "build/build_config.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "components/search_engines/template_url_service.h"
#include "extensions/browser/event_router.h"
#include "extensions/browser/extension_function.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_prefs_factory.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/extension_id.h"
#include "ui/gfx/image/image.h"
#include "base/android/build_info.h"
#include "base/json/json_writer.h"
#include "chrome/browser/wootz_wallet/wootz_wallet_service_factory.h"
#include "components/wootz_wallet/browser/wootz_wallet_service.h"

#include "extensions/browser/extension_event_histogram_value.h"

namespace extensions {

wootz_wallet::KeyringService* GetKeyringService(content::BrowserContext* context) {
  auto* profile = Profile::FromBrowserContext(context);
  return wootz_wallet::WootzWalletServiceFactory::GetServiceForContext(profile)
      ->keyring_service();
}


ExtensionFunction::ResponseAction WootzInfoFunction::Run() {
    const base::android::BuildInfo* build_info = base::android::BuildInfo::GetInstance();

    // Create a dictionary to store the relevant build information
    base::Value::Dict build_info_dict;
    build_info_dict.Set("device", build_info->device());
    build_info_dict.Set("manufacturer", build_info->manufacturer());
    build_info_dict.Set("model", build_info->model());
    build_info_dict.Set("brand", build_info->brand());
    build_info_dict.Set("sdk_int", build_info->sdk_int());
    build_info_dict.Set("android_build_id", build_info->android_build_id());
    build_info_dict.Set("android_build_fp", build_info->android_build_fp());
    build_info_dict.Set("gms_version_code", build_info->gms_version_code());
    build_info_dict.Set("host_package_name", build_info->host_package_name());
    build_info_dict.Set("package_name", build_info->package_name());
    build_info_dict.Set("package_version_code", build_info->package_version_code());
    build_info_dict.Set("package_version_name", build_info->package_version_name());
    build_info_dict.Set("abi_name", build_info->abi_name());
    build_info_dict.Set("is_tv", build_info->is_tv());
    build_info_dict.Set("is_automotive", build_info->is_automotive());
    build_info_dict.Set("is_foldable", build_info->is_foldable());

    std::string json_string;
    base::JSONWriter::Write(build_info_dict, &json_string);

    return RespondNow(WithArguments(json_string));
}
ExtensionFunction::ResponseAction WootzCreateWalletFunction::Run() {
  if (args().empty() || !args()[0].is_string()) {
    return RespondNow(Error("Invalid arguments"));
  }

  std::string password = args()[0].GetString();
  auto* keyring_service = GetKeyringService(browser_context());
  
  if (!keyring_service) {
    return RespondNow(Error("KeyringService not available"));
  }

  keyring_service->CreateWallet(
      password,
      base::BindOnce(&WootzCreateWalletFunction::OnWalletCreated, this));

  return RespondLater();
}

void WootzCreateWalletFunction::OnWalletCreated(
    const std::optional<std::string>& recovery_phrase) {
  base::Value::Dict result;
  result.Set("success", recovery_phrase.has_value());
  if (recovery_phrase) {
    result.Set("recoveryPhrase", *recovery_phrase);
  } else {
    result.Set("error", "Failed to create wallet");
  }
  
  base::Value::List result_list;
  result_list.Append(std::move(result));
  Respond(ArgumentList(std::move(result_list)));
}

ExtensionFunction::ResponseAction WootzUnlockWalletFunction::Run() {
  if (args().empty() || !args()[0].is_string()) {
    return RespondNow(Error("Invalid arguments"));
  }

  std::string password = args()[0].GetString();
  auto* keyring_service = GetKeyringService(browser_context());
  
  if (!keyring_service) {
    return RespondNow(Error("KeyringService not available"));
  }

  keyring_service->SetPassword(password);
  keyring_service->Unlock(
      password,
      base::BindOnce(&WootzUnlockWalletFunction::OnUnlocked, this));

  return RespondLater();
}

void WootzUnlockWalletFunction::OnUnlocked(bool success) {
  base::Value::Dict result;
  result.Set("success", success);
  if (!success) {
    result.Set("error", "Failed to unlock wallet");
  }
  
  base::Value::List result_list;
  result_list.Append(std::move(result));
  Respond(ArgumentList(std::move(result_list)));
}

ExtensionFunction::ResponseAction WootzGetAllAccountsFunction::Run() {
  auto* keyring_service = GetKeyringService(browser_context());
  
  if (!keyring_service) {
    return RespondNow(Error("KeyringService not available"));
  }

  keyring_service->GetAllAccounts(
      base::BindOnce(&WootzGetAllAccountsFunction::OnGetAllAccounts,
                     weak_factory_.GetWeakPtr()));

  return RespondLater();
}

void WootzGetAllAccountsFunction::OnGetAllAccounts(
    wootz_wallet::mojom::AllAccountsInfoPtr all_accounts_info) {
  LOG(ERROR) << "WootzGetAllAccounts: Got all accounts, count = " 
             << all_accounts_info->accounts.size();

  base::Value::Dict result;
  base::Value::List accounts_list;

  LOG(ERROR) << "WootzGetAllAccounts: Listing all accounts:";
  for (const auto& account : all_accounts_info->accounts) {
    base::Value::Dict account_dict;
    account_dict.Set("address", account->address);
    account_dict.Set("name", account->name);
    account_dict.Set("coin", static_cast<int>(account->account_id->coin));
    accounts_list.Append(std::move(account_dict));

    LOG(ERROR) << "  Address: " << account->address
               << ", Name: " << account->name
               << ", Coin: " << static_cast<int>(account->account_id->coin);
  }

  result.Set("accounts", std::move(accounts_list));
  result.Set("success", true);

  base::Value::List result_list;
  result_list.Append(std::move(result));
  Respond(ArgumentList(std::move(result_list)));
}

// static


void WootzSignMessageFunction::NotifyExtensionOfPendingRequest(
    content::BrowserContext* context) {
  LOG(ERROR) << "JANGID: NotifyExtensionOfPendingRequest: Starting";
  
  auto* service = wootz_wallet::WootzWalletServiceFactory::GetServiceForContext(
      Profile::FromBrowserContext(context));
  
  if (!service) {
    LOG(ERROR) << "JANGID: NotifyExtensionOfPendingRequest: Failed to get WootzWalletService";
    return;
  }
  
  LOG(ERROR) << "JANGID: NotifyExtensionOfPendingRequest: Calling GetPendingSignMessageRequests";
  service->GetPendingSignMessageRequests(base::BindOnce(
      &WootzSignMessageFunction::OnGetPendingRequests,
      base::Unretained(context)));
}

// static
void WootzSignMessageFunction::OnGetPendingRequests(
    content::BrowserContext* context,
    std::vector<wootz_wallet::mojom::SignMessageRequestPtr> requests) {
  LOG(ERROR) << "JANGID: OnGetPendingRequests: Starting with " << requests.size() << " requests";
  
  if (requests.empty()) {
    LOG(ERROR) << "JANGID: OnGetPendingRequests: No pending requests, returning";
    return;
  }

  const auto& request = requests[0];
  base::Value::Dict request_dict;
  request_dict.Set("id", request->id);
  request_dict.Set("address", request->account_id->address);
  // request_dict.Set("message", request->sign_data->message);
  request_dict.Set("origin", request->origin_info->origin_spec);
  request_dict.Set("chainId", request->chain_id);
  request_dict.Set("isEip712", request->sign_data->is_eth_sign_typed_data());

  // Log the request to console
  std::string json_string;
  base::JSONWriter::Write(request_dict, &json_string);
  LOG(ERROR) << "JANGID: OnGetPendingRequests: Pending sign message request: " << json_string;

  // Pass the request to the extension using an event
  auto* extension_system = ExtensionSystem::Get(context);
  if (extension_system) {
    auto* event_router = EventRouter::Get(context);
    if (event_router) {
      LOG(ERROR) << "JANGID: OnGetPendingRequests: Broadcasting event to extension";
      base::Value::List event_args;
      event_args.Append(std::move(request_dict));
      std::unique_ptr<Event> event = std::make_unique<Event>(
          events::WOOTZ_ON_SIGN_MESSAGE_REQUESTED,
          "wootz.onSignMessageRequested",
          std::move(event_args),
          context);
      event_router->BroadcastEvent(std::move(event));
      LOG(ERROR) << "JANGID: OnGetPendingRequests: Event broadcasted successfully";
    } else {
      LOG(ERROR) << "JANGID: OnGetPendingRequests: Failed to get EventRouter";
    }
  } else {
    LOG(ERROR) << "JANGID: OnGetPendingRequests: Failed to get ExtensionSystem";
  }
}

ExtensionFunction::ResponseAction WootzSignMessageFunction::Run() {
  LOG(ERROR) << "JANGID: WootzSignMessageFunction::Run: Starting";

  // Check if we have the correct number of arguments
  if (args().size() < 2 || args().size() > 3) {
    LOG(ERROR) << "JANGID: WootzSignMessageFunction::Run: Incorrect number of arguments. Expected 2 or 3, got " << args().size();
    return RespondNow(Error("Incorrect number of arguments"));
  }

  // Validate argument types
  if (!args()[0].is_int() || !args()[1].is_bool()) {
    LOG(ERROR) << "JANGID: WootzSignMessageFunction::Run: Invalid argument types";
    return RespondNow(Error("Invalid argument types"));
  }

  int request_id = args()[0].GetInt();
  bool approved = args()[1].GetBool();
  std::string signature;
  if (args().size() == 3 && args()[2].is_string()) {
    signature = args()[2].GetString();
  }

  LOG(ERROR) << "JANGID: WootzSignMessageFunction::Run: Request ID: " << request_id 
             << ", Approved: " << (approved ? "Yes" : "No")
             << ", Signature: " << (signature.empty() ? "Not provided" : "Provided");

  auto* service = wootz_wallet::WootzWalletServiceFactory::GetServiceForContext(
      Profile::FromBrowserContext(browser_context()));
  
  if (!service) {
    LOG(ERROR) << "JANGID: WootzSignMessageFunction::Run: Failed to get WootzWalletService";
    return RespondNow(Error("WootzWalletService not available"));
  }

  wootz_wallet::mojom::ByteArrayStringUnionPtr signature_ptr = nullptr;

  std::optional<std::string> error = 
      approved ? std::nullopt : std::make_optional<std::string>("User rejected");

  LOG(ERROR) << "JANGID: WootzSignMessageFunction::Run: Calling NotifySignMessageRequestProcessed";
  service->NotifySignMessageRequestProcessed(approved, request_id, std::move(signature_ptr), error);

  LOG(ERROR) << "JANGID: WootzSignMessageFunction::Run: Completed";
  return RespondNow(WithArguments(base::Value(true)));
}

}  // namespace extensions
