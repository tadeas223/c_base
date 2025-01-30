pkgname=c_base
pkgver=0.0.1
pkgrel=1
pkgdesc="Base library for C projects"
url="https://github.com/tadeas223/c_base/"
cksums=("SKIP")
arch=("x86_64")
depends=()
makedepends=("meson" "ninja")
source=("git+https://github.com/tadeas223/c_base.git")

build() {
    cd "$srcdir/$pkgname"
    meson setup build --prefix=/usr
    ninja -C build
}

package() {
    cd "$srcdir/$pkgname"
    DESTDIR="$pkgdir" ninja -C build install
    install "$srcdir/c_base.pc" "$pkgdir/usr/lib/pkgconfig/c_base.pc"
}
