Name: obex-capability
Version: 0.0.1
Release: 1
Summary: Simple tool for generating OBEX capability files
License: GPLv2
Source: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Xml)
BuildRequires: pkgconfig(systemsettings) >= 0.6.0

%description
%{summary}.

%prep
%setup -q

%build
%qmake5
make %{?_smp_mflags}

%install
make INSTALL_ROOT=%{buildroot} install

%files
%license COPYING
%{_bindir}/obex-capability
