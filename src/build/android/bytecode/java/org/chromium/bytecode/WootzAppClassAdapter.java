/* Copyright (c) 2025 The WootzApp Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.bytecode;

import org.objectweb.asm.ClassVisitor;

/** Adapter to perform Java asm patches on upstreams' classes. */
public class WootzAppClassAdapter {
    public static ClassVisitor createAdapter(ClassVisitor chain) {
        chain = new WootzAppNewTabPageClassAdapter(chain);
        return chain;
    }
}
