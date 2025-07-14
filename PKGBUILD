pkgname=c_base
pkgver=1.0.0
pkgrel=1
pkgdesc="Base library for C projects"
arch=('x86_64')
url="https://github.com/tadeas223/c_base"
license=('MIT')
depends=()
makedepends=('meson' 'ninja' 'gcc' 'pkgconf')
source=("git+https://github.com/tadeas223/c_base.git")
sha256sums=('SKIP')

build() {
  meson setup build --prefix=/usr
  meson compile -C build
}

package() {
  meson install -C build --destdir="$pkgdir"
}
