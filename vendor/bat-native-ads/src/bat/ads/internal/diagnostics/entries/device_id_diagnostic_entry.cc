/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/diagnostics/entries/device_id_diagnostic_entry.h"

#include "bat/ads/ads.h"

namespace ads {

namespace {

constexpr char kName[] = "Device Id";
constexpr char kUnsupported[] = "Unsupported";

}  // namespace

DeviceIdDiagnosticEntry::DeviceIdDiagnosticEntry() = default;

DeviceIdDiagnosticEntry::~DeviceIdDiagnosticEntry() = default;

DiagnosticEntryType DeviceIdDiagnosticEntry::GetType() const {
  return DiagnosticEntryType::kDeviceId;
}

std::string DeviceIdDiagnosticEntry::GetName() const {
  return kName;
}

std::string DeviceIdDiagnosticEntry::GetValue() const {
  if (SysInfo().device_id.empty()) {
    return kUnsupported;
  }

  return SysInfo().device_id;
}

}  // namespace ads
