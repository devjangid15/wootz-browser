// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <algorithm>

#include "components/wootz_adblock/core/browser/filter_list_catalog_entry.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(AdBlockComponentServiceTest, UserModelLanguages) {
  std::vector<wootz_adblock::FilterListCatalogEntry> catalog;
  catalog.push_back(wootz_adblock::FilterListCatalogEntry(
      "uuid", "https://brave.com", "Testing Filter List #1", {"fr"},
      "https://support.brave.com", "Filter list for testing purposes", false,
      false, false, 0, {}, "componentid", "base64publickey"));
  catalog.push_back(wootz_adblock::FilterListCatalogEntry(
      "uuid", "https://brave.com", "Testing Filter List #2", {"en"},
      "https://support.brave.com", "Filter list for testing purposes", false,
      false, false, 0, {}, "componentid", "base64publickey"));
  catalog.push_back(wootz_adblock::FilterListCatalogEntry(
      "uuid", "https://brave.com", "Testing Filter List #2", {"fr"},
      "https://support.brave.com", "Filter list for testing purposes", false,
      false, false, 0, {}, "componentid", "base64publickey"));

  std::vector<std::string> languages({"fr", "fR", "fr-FR", "fr-ca"});
  std::for_each(
      languages.begin(), languages.end(), [&](const std::string& language) {
        EXPECT_EQ(
            wootz_adblock::FindAdBlockFilterListsByLocale(catalog, language)
                .size(),
            2UL);
      });

  EXPECT_EQ(wootz_adblock::FindAdBlockFilterListsByLocale(catalog, "en").size(),
            1UL);

  EXPECT_EQ(wootz_adblock::FindAdBlockFilterListsByLocale(catalog, "is").size(),
            0UL);
}

TEST(AdBlockComponentServiceTest, MissingFieldDefaultValues) {
  auto catalog = wootz_adblock::FilterListCatalogFromJSON(R"([{
      "uuid": "uuid",
      "title": "Test list",
      "desc": "Just used for testing",
      "langs": [],
      "component_id": "componentid",
      "base64_public_key": "base64publickey",
      "list_text_component": {
          "component_id": "componentid",
          "base64_public_key": "base64publickey"
      },
      "sources": [
          {
              "url": "https://example.com",
              "format": "Standard",
              "support_url": "https://support.example.com"
          }
      ]
  }])");

  ASSERT_EQ(catalog.size(), 1UL);
  ASSERT_EQ(catalog[0].hidden, false);
  ASSERT_EQ(catalog[0].default_enabled, false);
  ASSERT_EQ(catalog[0].first_party_protections, false);
  ASSERT_EQ(catalog[0].permission_mask, 0);
  ASSERT_EQ(catalog[0].platforms.size(), 0UL);
}
