[Defines]
!ifdef $(DEB_VERSION)
  DEFINE COMPILE_DEB_VERSION        = $(DEB_VERSION)
!else
  DEFINE COMPILE_DEB_VERSION        = 0.0.1
!endif