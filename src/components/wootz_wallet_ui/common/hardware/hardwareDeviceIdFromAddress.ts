// Copyright (c) 2021 The Wootz Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
const crypto = require('crypto')

export async function hardwareDeviceIdFromAddress(address: string | Buffer) {
  return crypto.createHash('sha256').update(address).digest('hex')
}
