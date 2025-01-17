// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "appsmodel.h"
#include "categoryutils.h"

#include <QDebug>
#include <DPinyin>
#include <appinfo.h>
#include <appinfomonitor.h>

AppsModel::AppsModel(QObject *parent)
    : QStandardItemModel(parent)
    , m_appInfoMonitor(new AppInfoMonitor(this))
{
    QHash<int, QByteArray> defaultRoleNames = roleNames();
    defaultRoleNames.insert({
        {AppItem::DesktopIdRole, QByteArrayLiteral("desktopId")},
        {AppItem::DDECategoryRole, QByteArrayLiteral("category")},
        {AppItem::IconNameRole, QByteArrayLiteral("iconName")},
        {AppsModel::TransliteratedRole, QByteArrayLiteral("transliterated")}
    });
    setItemRoleNames(defaultRoleNames);

    QList<AppItem *> items(allAppInfosShouldBeShown());
    QList<AppItem *> duplicatedItems = addItems(items);
    Q_ASSERT(duplicatedItems.isEmpty());
    qDebug() << rowCount();

    connect(m_appInfoMonitor, &AppInfoMonitor::changed, this, [this](){
        qDebug() << "changed";
        QList<AppItem *> items(allAppInfosShouldBeShown());
        QList<AppItem *> duplicatedItems = updateItems(items);
        for (AppItem * item : qAsConst(duplicatedItems)) {
            delete item;
        }
    });
}

void AppsModel::appendRows(const QList<AppItem *> items)
{
    // TODO: preformance improvement?
    for (AppItem * item : items) {
        appendRow(item);
    }
}

AppItem *AppsModel::itemFromDesktopId(const QString freedesktopId)
{
    QModelIndexList indexes = match(index(0, 0, QModelIndex()),
                                    AppItem::DesktopIdRole, freedesktopId, 1, Qt::MatchExactly);

    if (indexes.isEmpty()) return nullptr;

    return static_cast<AppItem *>(itemFromIndex(indexes.at(0)));
}

// the model takes the ownership for the items that actually added to the model.
// won't try to update item if there are existing ones.
// return the duplicated ones
const QList<AppItem *> AppsModel::addItems(const QList<AppItem *> &items)
{
    QList<AppItem *> append;
    QList<AppItem *> duplicated;

    for (AppItem * item : items) {
        if (itemFromDesktopId(item->freedesktopId()) != nullptr) {
            duplicated.append(item);
        } else {
            append.append(item);
        }
    }

    appendRows(append);
    return duplicated;
}

// try to update items, if not exist, add them to the model.
// the reference item that were used to update the existing one won't replace the existing one, thus
// they will be in the returned item list.
// return the ones were not added to the model.
const QList<AppItem *> AppsModel::updateItems(const QList<AppItem *> &items)
{
    QList<AppItem *> append;
    QList<AppItem *> duplicated;

    for (AppItem * item : items) {
        AppItem * existing = itemFromDesktopId(item->freedesktopId());
        if (existing != nullptr) {
            existing->updateData(item);
            if (existing != item) {
                duplicated.append(item);
            }
        } else {
            append.append(item);
        }
    }

    appendRows(append);
    return duplicated;
}

QVariant AppsModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case AppsModel::TransliteratedRole:
        // TODO: 1. use icu::Transliterator for other locales
        //       2. support polyphonic characters (e.g. Music: YinYue or YinLe)
        return Dtk::Core::pinyin(index.data(Qt::DisplayRole).toString(), Dtk::Core::TS_NoneTone).constFirst();
    default:
        break;
    }

    return QStandardItemModel::data(index, role);
}

// the caller manage the return values' ownership (i.e. might need to free them)
QList<AppItem *> AppsModel::allAppInfosShouldBeShown() const
{
    const auto list = m_appInfoMonitor->allAppInfosShouldBeShown();
    QList<AppItem *> items;
    for (const QHash<QString, QString> & hashmap : list) {
        auto item = new AppItem(hashmap["id"]);
        item->setDisplayName(hashmap["name"]);
        item->setIconName(hashmap["icon"]);
        item->setCategories(hashmap["categories"].split(';'));
        item->setDDECategory(AppItem::DDECategories(CategoryUtils::parseBestMatchedCategory(hashmap["categories"].split(';'))));
        items.append(item);
    }
    return items;
}
