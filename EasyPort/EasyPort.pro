
QT       += core gui network

TARGET = EasyPort
target.path = /usr/local/bin

TEMPLATE = app


SOURCES += main.cpp \
        mainUI.cpp 

HEADERS  += mainUI.h 

FORMS    += mainUI.ui 

RESOURCES+= EasyPort.qrc

LIBS     += -lQtSolutions_SingleApplication-head

TRANSLATIONS =  i18n/EasyPort_af.ts \
                i18n/EasyPort_ar.ts \
                i18n/EasyPort_az.ts \
                i18n/EasyPort_bg.ts \
                i18n/EasyPort_bn.ts \
                i18n/EasyPort_bs.ts \
                i18n/EasyPort_ca.ts \
                i18n/EasyPort_cs.ts \
                i18n/EasyPort_cy.ts \
                i18n/EasyPort_da.ts \
                i18n/EasyPort_de.ts \
                i18n/EasyPort_el.ts \
                i18n/EasyPort_en_GB.ts \
                i18n/EasyPort_en_ZA.ts \
                i18n/EasyPort_es.ts \
                i18n/EasyPort_et.ts \
                i18n/EasyPort_eu.ts \
                i18n/EasyPort_fa.ts \
                i18n/EasyPort_fi.ts \
                i18n/EasyPort_fr.ts \
                i18n/EasyPort_fr_CA.ts \
                i18n/EasyPort_gl.ts \
                i18n/EasyPort_he.ts \
                i18n/EasyPort_hi.ts \
                i18n/EasyPort_hr.ts \
                i18n/EasyPort_hu.ts \
                i18n/EasyPort_id.ts \
                i18n/EasyPort_is.ts \
                i18n/EasyPort_it.ts \
                i18n/EasyPort_ja.ts \
                i18n/EasyPort_ka.ts \
                i18n/EasyPort_ko.ts \
                i18n/EasyPort_lt.ts \
                i18n/EasyPort_lv.ts \
                i18n/EasyPort_mk.ts \
                i18n/EasyPort_mn.ts \
                i18n/EasyPort_ms.ts \
                i18n/EasyPort_mt.ts \
                i18n/EasyPort_nb.ts \
                i18n/EasyPort_nl.ts \
                i18n/EasyPort_pa.ts \
                i18n/EasyPort_pl.ts \
                i18n/EasyPort_pt.ts \
                i18n/EasyPort_pt_BR.ts \
                i18n/EasyPort_ro.ts \
                i18n/EasyPort_ru.ts \
                i18n/EasyPort_sa.ts \
                i18n/EasyPort_sk.ts \
                i18n/EasyPort_sl.ts \
                i18n/EasyPort_sr.ts \
                i18n/EasyPort_sv.ts \
                i18n/EasyPort_sw.ts \
                i18n/EasyPort_ta.ts \
                i18n/EasyPort_tg.ts \
                i18n/EasyPort_th.ts \
                i18n/EasyPort_tr.ts \
                i18n/EasyPort_uk.ts \
                i18n/EasyPort_uz.ts \
                i18n/EasyPort_vi.ts \
                i18n/EasyPort_zh_CN.ts \
                i18n/EasyPort_zh_HK.ts \
                i18n/EasyPort_zh_TW.ts \
                i18n/EasyPort_zu.ts

dotrans.path=/usr/local/share/EasyPort/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/EasyPort/i18n/

desktop.path=/usr/local/share/applications/
desktop.files=EasyPort.desktop

icons.path=/usr/local/share/EasyPort/
icons.files=icons/EasyPorticon.png

INSTALLS += target desktop icons dotrans
