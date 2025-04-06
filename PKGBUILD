pkgname=( "c_base" "c_base-static")
pkgver=0.0.1
pkgrel=1
url="https://github.com/tadeas223/c_base/"
cksums=("SKIP")
arch=("x86_64")
depends=()
makedepends=("meson" "ninja")
source=("git+https://github.com/tadeas223/c_base.git")

build() {
    cd "$srcdir/$pkgname"
    meson setup build --prefix=/usr -Ddefault_library=both
    meson compile -C build
}

package_c_base() {
    pkgdesc="Shared base library for C projects"
    cd "$srcdir/$pkgname"
    DESTDIR="$pkgdir" meson install -C build
    rm -f "$pkgdir/usr/lib/lib_c_base.a"
}

package_c_base-static() {
    pkgdesc="Static base library for C projects"
    cd "$srcdir/$pkgname"
    DESTDIR="$pkgdir" meson install -C build
    rm -f "$pkgdir/usr/lib/lib_c_base.so"
}
