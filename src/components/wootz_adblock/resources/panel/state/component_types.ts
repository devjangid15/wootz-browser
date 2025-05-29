// Copyright (c) 2025 The WootzApp Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

export enum ViewType {
  ScriptsList,
  FingerprintList,
  AdsList,
  HttpsList,
  Main
}

export type PermissionButtonHandler = ((name: string) => void)
