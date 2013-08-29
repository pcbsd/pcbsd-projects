QT       += core gui

TARGET = pc-devicemanager
target.path = /usr/local/bin

TEMPLATE = app


SOURCES += main.cpp mainUI.cpp 


HEADERS  += mainUI.h 


FORMS    += mainUI.ui 


RESOURCES+= pc-devicemanager.qrc

LIBS     += -lQtSolutions_SingleApplication-head

TRANSLATIONS =  i18n/pc-devicemanager_af.ts \
                i18n/pc-devicemanager_ar.ts \
                i18n/pc-devicemanager_az.ts \
                i18n/pc-devicemanager_bg.ts \
                i18n/pc-devicemanager_bn.ts \
                i18n/pc-devicemanager_bs.ts \
                i18n/pc-devicemanager_ca.ts \
                i18n/pc-devicemanager_cs.ts \
                i18n/pc-devicemanager_cy.ts \
                i18n/pc-devicemanager_da.ts \
                i18n/pc-devicemanager_de.ts \
                i18n/pc-devicemanager_el.ts \
                i18n/pc-devicemanager_en_GB.ts \
                i18n/pc-devicemanager_en_ZA.ts \
                i18n/pc-devicemanager_es.ts \
                i18n/pc-devicemanager_et.ts \
                i18n/pc-devicemanager_eu.ts \
                i18n/pc-devicemanager_fa.ts \
                i18n/pc-devicemanager_fi.ts \
                i18n/pc-devicemanager_fr.ts \
                i18n/pc-devicemanager_fr_CA.ts \
                i18n/pc-devicemanager_gl.ts \
                i18n/pc-devicemanager_he.ts \
                i18n/pc-devicemanager_hi.ts \
                i18n/pc-devicemanager_hr.ts \
                i18n/pc-devicemanager_hu.ts \
                i18n/pc-devicemanager_id.ts \
                i18n/pc-devicemanager_is.ts \
                i18n/pc-devicemanager_it.ts \
                i18n/pc-devicemanager_ja.ts \
                i18n/pc-devicemanager_ka.ts \
                i18n/pc-devicemanager_ko.ts \
                i18n/pc-devicemanager_lt.ts \
                i18n/pc-devicemanager_lv.ts \
                i18n/pc-devicemanager_mk.ts \
                i18n/pc-devicemanager_mn.ts \
                i18n/pc-devicemanager_ms.ts \
                i18n/pc-devicemanager_mt.ts \
                i18n/pc-devicemanager_nb.ts \
                i18n/pc-devicemanager_nl.ts \
                i18n/pc-devicemanager_pa.ts \
                i18n/pc-devicemanager_pl.ts \
                i18n/pc-devicemanager_pt.ts \
                i18n/pc-devicemanager_pt_BR.ts \
                i18n/pc-devicemanager_ro.ts \
                i18n/pc-devicemanager_ru.ts \
                i18n/pc-devicemanager_sa.ts \
                i18n/pc-devicemanager_sk.ts \
                i18n/pc-devicemanager_sl.ts \
                i18n/pc-devicemanager_sr.ts \
                i18n/pc-devicemanager_sv.ts \
                i18n/pc-devicemanager_sw.ts \
                i18n/pc-devicemanager_ta.ts \
                i18n/pc-devicemanager_tg.ts \
                i18n/pc-devicemanager_th.ts \
                i18n/pc-devicemanager_tr.ts \
                i18n/pc-devicemanager_uk.ts \
                i18n/pc-devicemanager_uz.ts \
                i18n/pc-devicemanager_vi.ts \
                i18n/pc-devicemanager_zh_CN.ts \
                i18n/pc-devicemanager_zh_HK.ts \
                i18n/pc-devicemanager_zh_TW.ts \
                i18n/pc-devicemanager_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm /usr/local/share/pcbsd/i18n/

icons.files = pc-devicemanager.png
icons.path=/usr/local/share/pcbsd/icons/

INSTALLS += target icons dotrans
