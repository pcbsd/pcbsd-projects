
TARGET = PCDM-session
target.path=/usr/local/bin
TEMPLATE = app
LIBS += -lpcbsd -lpam
SOURCES += main.cpp \
           pcdm-gui.cpp \
    	   pcdm-backend.cpp \
    	   themeStruct.cpp \
           pcdm-config.cpp \
	   pcdm-xprocess.cpp \
           fancySwitcher.cpp \
           dialogKeyboard.cpp \
           dialogLocale.cpp \
           loginWidget.cpp

HEADERS += pcdm-gui.h \
    	   pcdm-backend.h \
	   themeStruct.h \
	   themeItem.h \
           pcdm-config.h \
	   pcdm-xprocess.h \
           fancySwitcher.h \
           dialogKeyboard.h \
           dialogLocale.h \
           loginWidget.h
           
FORMS += dialogKeyboard.ui dialogLocale.ui

TRANSLATIONS =  i18n/PCDM_af.ts \
		i18n/PCDM_ar.ts \
		i18n/PCDM_az.ts \
		i18n/PCDM_bg.ts \
		i18n/PCDM_bn.ts \
		i18n/PCDM_bs.ts \
		i18n/PCDM_ca.ts \
		i18n/PCDM_cs.ts \
		i18n/PCDM_cy.ts \
		i18n/PCDM_da.ts \
		i18n/PCDM_de.ts \
		i18n/PCDM_el.ts \
		i18n/PCDM_es.ts \
		i18n/PCDM_en.ts \
		i18n/PCDM_en_US.ts \
		i18n/PCDM_en_GB.ts \
		i18n/PCDM_en_ZA.ts \
		i18n/PCDM_et.ts \
		i18n/PCDM_eu.ts \
		i18n/PCDM_fa.ts \
		i18n/PCDM_fi.ts \
		i18n/PCDM_fr.ts \
		i18n/PCDM_fr_CA.ts \
		i18n/PCDM_fur.ts \
		i18n/PCDM_gl.ts \
		i18n/PCDM_he.ts \
		i18n/PCDM_hi.ts \
		i18n/PCDM_hr.ts \
		i18n/PCDM_hu.ts \
		i18n/PCDM_id.ts \
		i18n/PCDM_is.ts \
		i18n/PCDM_it.ts \
		i18n/PCDM_ja.ts \
		i18n/PCDM_ka.ts \
		i18n/PCDM_ko.ts \
		i18n/PCDM_lt.ts \
		i18n/PCDM_lv.ts \
		i18n/PCDM_mk.ts \
		i18n/PCDM_mn.ts \
		i18n/PCDM_ms.ts \
		i18n/PCDM_mt.ts \
		i18n/PCDM_nb.ts \
		i18n/PCDM_ne.ts \
		i18n/PCDM_nl.ts \
		i18n/PCDM_pa.ts \
		i18n/PCDM_pl.ts \
		i18n/PCDM_pt.ts \
		i18n/PCDM_pt_BR.ts \
		i18n/PCDM_ro.ts \
		i18n/PCDM_ru.ts \
		i18n/PCDM_sk.ts \
		i18n/PCDM_sl.ts \
		i18n/PCDM_sr.ts \
		i18n/PCDM_sv.ts \
		i18n/PCDM_sw.ts \
		i18n/PCDM_ta.ts \
		i18n/PCDM_tg.ts \
		i18n/PCDM_th.ts \
		i18n/PCDM_tr.ts \
		i18n/PCDM_uk.ts \
		i18n/PCDM_uz.ts \
		i18n/PCDM_vi.ts \
		i18n/PCDM_zh_CN.ts \
		i18n/PCDM_zh_HK.ts \
		i18n/PCDM_zh_TW.ts \
		i18n/PCDM_zu.ts

dotrans.path=/usr/local/share/PCDM/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm /usr/local/share/PCDM/i18n/

scripts=PCDMd
scripts.path=/usr/local/sbin
scripts.extra=cp PCDMd /usr/local/sbin/PCDMd

rcd=rc.d/pcdm
rcd.path=/usr/local/etc/rc.d
rcd.extra=cp rc.d/pcdm /usr/local/etc/rc.d/pcdm

cleanthemes.path=/usr/local/share/PCDM/themes
cleanthemes.extra=rm -r /usr/local/share/PCDM/themes

theme=themes
theme.path=/usr/local/share/PCDM
theme.extra=cp -r themes /usr/local/share/PCDM/.

INSTALLS += dotrans scripts rcd cleanthemes theme target

RESOURCES += PCDM.qrc
