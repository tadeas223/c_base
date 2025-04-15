pkgname=( "c_base" "c_base-static")
pkgver=0.0.1
pkgrel=1
url="https://github.com/tadeas223/c_base/"
cksums=("SKIP")
arch=("x86_64")
makedepends=("meson" "ninja")
source=("git+https://github.com/tadeas223/c_base.git")

build_c_base() {
    cd "$srcdir/c_base"
    meson setup build --prefix=/usr -Ddefault_library=both
    meson compile -C build
}

package_c_base() {
    pkgdesc="Shared base library for C projects"
    depends=()

    cd "$srcdir/c_base"
    DESTDIR="$pkgdir" meson install -C build
    rm "$pkgdir/usr/lib/libc_base.a"
}

package_c_base-static() {
    pkgdesc="Static base library for C projects"
    depends=('c_base')
    
    cd "$srcdir/c_base"
    DESTDIR="$pkgdir" meson install -C build

    rm "$pkgdir/usr/lib/libc_base.so"
    rm -r "$pkgdir/usr/lib/pkgconfig/"
    rm -r "$pkgdir/usr/include/"
}
