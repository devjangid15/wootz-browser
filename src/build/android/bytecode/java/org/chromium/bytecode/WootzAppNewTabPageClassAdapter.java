package org.chromium.bytecode;

import static org.objectweb.asm.Opcodes.ASM7;

import org.objectweb.asm.ClassVisitor;

public class WootzAppNewTabPageClassAdapter extends WootzAppClassVisitor {
    static String sNewTabPageClassName = "org/chromium/chrome/browser/ntp/NewTabPage";
    static String sWootzAppNewTabPageClassName = "org/chromium/chrome/browser/ntp/WootzAppNewTabPage";

    public WootzAppNewTabPageClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sNewTabPageClassName, sWootzAppNewTabPageClassName);

        deleteField(sWootzAppNewTabPageClassName, "mNewTabPageLayout");
        makeProtectedField(sNewTabPageClassName, "mNewTabPageLayout");

        makeProtectedField(sNewTabPageClassName, "mFeedSurfaceProvider");
        deleteField(sWootzAppNewTabPageClassName, "mFeedSurfaceProvider");
        
        makePublicMethod(sNewTabPageClassName, "updateSearchProviderHasLogo");
        addMethodAnnotation(
            sWootzAppNewTabPageClassName, "updateSearchProviderHasLogo", "Ljava/lang/Override;");
    }
}
