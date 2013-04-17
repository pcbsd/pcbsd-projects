#-------------------------------------------------
#
# Project created by QtCreator 2011-10-25T20:47:28
#
#-------------------------------------------------

QT       += core gui network

TARGET = pc-repomanager
target.path = /usr/local/bin

TEMPLATE = app


SOURCES += main.cpp \
		downloadWidget.cpp

HEADERS  += downloadWidget.h

FORMS    += mainGUI.ui

RESOURCES+= pc-repomanager.qrc

LIBS     += -lQtSolutions_SingleApplication-head

TRANSLATIONS =  i18n/pcbsd_af.ts \
                i18n/pcbsd_ar.ts \
                i18n/pcbsd_az.ts \
                i18n/pcbsd_bg.ts \
                i18n/pcbsd_bn.ts \
                i18n/pcbsd_bs.ts \
                i18n/pcbsd_ca.ts \
                i18n/pcbsd_cs.ts \
                i18n/pcbsd_cy.ts \
                i18n/pcbsd_da.ts \
                i18n/pcbsd_de.ts \
                i18n/pcbsd_el.ts \
                i18n/pcbsd_en_GB.ts \
                i18n/pcbsd_en_ZA.ts \
                i18n/pcbsd_es.ts \
                i18n/pcbsd_et.ts \
                i18n/pcbsd_eu.ts \
                i18n/pcbsd_fa.ts \
                i18n/pcbsd_fi.ts \
                i18n/pcbsd_fr.ts \
                i18n/pcbsd_fr_CA.ts \
                i18n/pcbsd_gl.ts \
                i18n/pcbsd_he.ts \
                i18n/pcbsd_hi.ts \
                i18n/pcbsd_hr.ts \
                i18n/pcbsd_hu.ts \
                i18n/pcbsd_id.ts \
                i18n/pcbsd_is.ts \
                i18n/pcbsd_it.ts \
                i18n/pcbsd_ja.ts \
                i18n/pcbsd_ka.ts \
                i18n/pcbsd_ko.ts \
                i18n/pcbsd_lt.ts \
                i18n/pcbsd_lv.ts \
                i18n/pcbsd_mk.ts \
                i18n/pcbsd_mn.ts \
                i18n/pcbsd_ms.ts \
                i18n/pcbsd_mt.ts \
                i18n/pcbsd_nb.ts \
                i18n/pcbsd_nl.ts \
                i18n/pcbsd_pa.ts \
                i18n/pcbsd_pl.ts \
                i18n/pcbsd_pt.ts \
                i18n/pcbsd_pt_BR.ts \
                i18n/pcbsd_ro.ts \
                i18n/pcbsd_ru.ts \
                i18n/pcbsd_sk.ts \
                i18n/pcbsd_sl.ts \
                i18n/pcbsd_sr.ts \
                i18n/pcbsd_sv.ts \
                i18n/pcbsd_sw.ts \
                i18n/pcbsd_ta.ts \
                i18n/pcbsd_tg.ts \
                i18n/pcbsd_th.ts \
                i18n/pcbsd_tr.ts \
                i18n/pcbsd_uk.ts \
                i18n/pcbsd_uz.ts \
                i18n/pcbsd_vi.ts \
                i18n/pcbsd_zh_CN.ts \
                i18n/pcbsd_zh_HK.ts \
                i18n/pcbsd_zh_TW.ts \
                i18n/pcbsd_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm /usr/local/share/pcbsd/i18n/

#desktop.path=/usr/local/share/applications/
#desktop.files=pc-repomanager.desktop

#icons.path=/usr/local/share/EasyPBI/
#icons.files=icons/EasyPBIicon.png

INSTALLS += target dotrans
