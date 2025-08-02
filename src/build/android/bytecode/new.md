This PR introduces a custom bytecode rewriting step into the Chromium Android build system. The new pipeline enables post-compilation bytecode transformations on Java `.jar` files, supporting advanced use cases such as instrumentation, code patching, and custom build-time modifications.

---

## Why Bytecode Rewriting After Compilation?

In the Chromium Android build, many classes and fields are private or package-private for encapsulation and security. However, when you create new classes by extending existing ones—especially if you want to customize or override upstream behavior—you often need access to fields or methods that are not public, or you may need to substitute your own implementations in place of upstream classes.

Bytecode rewriting is essential in this context because it allows us to:
- Change access modifiers (e.g., make private/protected fields or methods accessible to subclasses).
- Redirect constructor calls so that custom subclasses are instantiated instead of the original upstream classes.
- Remove or add fields and methods as needed for custom implementations.
- Add annotations or other metadata required for tooling or runtime behavior.

>**Note:**
>- After compilation, the Java compiler generates new `.class` files for these subclasses.
>- To ensure that any required bytecode transformations (such as those listed above) are applied to your new classes, they must be included as inputs to the bytecode processor.
>- This guarantees that the new classes are correctly transformed and packaged in the output `.jar`. 
>- **Failing to do so can result in runtime errors, such as `ClassNotFoundException`, `IllegalAccessError`, or missing method implementations, because your new classes may depend on modifications made to the upstream classes by the bytecode rewriter**.

---

## What's Included

### 1. Bytecode Processor (Java)

- **File:** `build/android/bytecode/java/org/chromium/bytecode/ByteCodeProcessor.java`
- **Description:**
  - Reads input `.jar` files, applies bytecode transformations using ASM, and writes out processed `.jar` files.
  - Designed to be invoked as a standalone Java application from the build system.
  - **Important:** If you create new classes by extending existing ones, you must ensure these new `.class` files are included as inputs to the bytecode processor. This guarantees that any required bytecode transformations are applied and that the new classes are packaged correctly in the output `.jar`. Failing to do so can result in runtime errors, such as `ClassNotFoundException` or missing method implementations.

### 2. Python Wrapper Script

- **File:** `build/android/gyp/wootzapp_bytecode_processor.py`
- **Description:**
  - Handles argument parsing, expands `@FileArg` references, and invokes the Java bytecode processor.
  - Ensures correct classpath and input/output jar handling.
  - Designed for use as a GN/Ninja `action_with_pydeps` step.

### 3. Build Integration

- **Files:** `build/android/bytecode/bytecode_rewriter.gni`, `build/android/bytecode/BUILD.gn`
- **Description:**
  - Defines GN templates and build rules for invoking the bytecode rewriter as part of the Android build.
  - Allows easy integration of the bytecode rewriting step for any Java target.

### 4. Supporting Files

- **File:** `build/android/gyp/wootzapp_bytecode_processor.pydeps`
- **Description:** Dependency file for the Python wrapper, ensuring correct build graph tracking.
