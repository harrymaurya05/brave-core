/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_UTILITY_IMPORTER_OPERA_IMPORTER_H_
#define BRAVE_UTILITY_IMPORTER_OPERA_IMPORTER_H_

#include "brave/utility/importer/chrome_importer.h"

class OperaImporter : public ChromeImporter {
 public:
  OperaImporter() = default;
  OperaImporter(const OperaImporter&) = delete;
  OperaImporter& operator=(const OperaImporter&) = delete;

 protected:
  ~OperaImporter() override {}
};

#endif  // BRAVE_UTILITY_IMPORTER_OPERA_IMPORTER_H_
