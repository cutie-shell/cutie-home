#include "notifications.h"

#include <QQuickView>
#include <QQuickItem>

Notifications::Notifications(QObject *parent)
	: QAbstractListModel(parent)
{
}

QStringList Notifications::GetCapabilities()
{
	QStringList caps = QStringList();
	caps.append("body");
	caps.append("persistence");
	return caps;
}

uint Notifications::Notify(QString app_name, uint replaces_id, QString app_icon,
			   QString summary, QString body, QStringList actions,
			   QVariantMap hints, int expire_timeout)
{
	uint notificationId = (replaces_id != 0) ? replaces_id : ++currentId;

	// If replacing an existing notification, find and update it
	if (replaces_id != 0) {
		for (int i = 0; i < m_notifications.size(); ++i) {
			if (m_notifications[i].id == replaces_id) {
				m_notifications[i] = {
					notificationId, app_name,      app_icon,
					summary,	body,	       actions,
					hints,		expire_timeout
				};
				QModelIndex index = createIndex(i, 0);
				emit dataChanged(index, index);
				emit notificationAdded(notificationId, summary,
						       body);
				return notificationId;
			}
		}
	}

	// Add new notification
	beginInsertRows(QModelIndex(), m_notifications.size(),
			m_notifications.size());
	m_notifications.append({ notificationId, app_name, app_icon, summary,
				 body, actions, hints, expire_timeout });
	endInsertRows();

	emit notificationAdded(notificationId, summary, body);

	return notificationId;
}

void Notifications::CloseNotification(uint id)
{
	for (int i = 0; i < m_notifications.size(); ++i) {
		if (m_notifications[i].id == id) {
			beginRemoveRows(QModelIndex(), i, i);
			m_notifications.removeAt(i);
			endRemoveRows();
			emit NotificationClosed(id, CloseReason::REQUESTED);
			break;
		}
	}
}

QString Notifications::GetServerInformation(QString &vendor, QString &version,
					    QString &spec_version)
{
	vendor = "Cutie Community Project";
	version = "0.0.1";
	spec_version = "1.2";
	return "Cutie Shell";
}

void Notifications::dismissNotification(uint id)
{
	for (int i = 0; i < m_notifications.size(); ++i) {
		if (m_notifications[i].id == id) {
			beginRemoveRows(QModelIndex(), i, i);
			m_notifications.removeAt(i);
			endRemoveRows();
			emit NotificationClosed(id, CloseReason::DISMISSED);
			break;
		}
	}
}

int Notifications::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_notifications.size();
}

QVariant Notifications::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() >= m_notifications.size())
		return QVariant();

	const NotificationData &notification = m_notifications.at(index.row());

	switch (role) {
	case IdRole:
		return notification.id;
	case AppNameRole:
		return notification.appName;
	case AppIconRole:
		return notification.appIcon;
	case SummaryRole:
		return notification.summary;
	case BodyRole:
		return notification.body;
	case ActionsRole:
		return notification.actions;
	case HintsRole:
		return notification.hints;
	case ExpireTimeoutRole:
		return notification.expireTimeout;
	default:
		return QVariant();
	}
}

QHash<int, QByteArray> Notifications::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[IdRole] = "id";
	roles[AppNameRole] = "appName";
	roles[AppIconRole] = "appIcon";
	roles[SummaryRole] = "summary";
	roles[BodyRole] = "body";
	roles[ActionsRole] = "actions";
	roles[HintsRole] = "hints";
	roles[ExpireTimeoutRole] = "expireTimeout";
	return roles;
}
