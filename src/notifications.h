#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QAbstractListModel>
#include <QMap>
#include <QVariant>

class Notifications : public QAbstractListModel {
	Q_OBJECT

    public:
	enum CloseReason {
		EXPIRED = 1,
		DISMISSED = 2,
		REQUESTED = 3,
		UNDEFINED = 4
	};
	Q_ENUM(CloseReason)

	enum Roles {
		IdRole = Qt::UserRole + 1,
		AppNameRole,
		AppIconRole,
		SummaryRole,
		BodyRole,
		ActionsRole,
		HintsRole,
		ExpireTimeoutRole
	};
	Q_ENUM(Roles)

	struct NotificationData {
		uint id;
		QString appName;
		QString appIcon;
		QString summary;
		QString body;
		QStringList actions;
		QVariantMap hints;
		int expireTimeout;
	};

	explicit Notifications(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index,
		      int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void dismissNotification(uint id);

    signals:
	void NotificationClosed(uint id, uint reason);
	void ActionInvoked(uint id, QString action);
	void ActivationToken(uint id, QString token);

	void notificationAdded(uint id, QString summary, QString body,
			       QVariantMap feedbackHints);

    public slots:
	QStringList GetCapabilities();
	uint Notify(QString app_name, uint replaces_id, QString app_icon,
		    QString summary, QString body, QStringList actions,
		    QVariantMap hints, int expire_timeout);
	void CloseNotification(uint id);
	QString GetServerInformation(QString &vendor, QString &version,
				     QString &spec_version);

    private:
	int currentId = 0;
	QList<NotificationData> m_notifications;

	QVariantMap
	convertToFeedbackHints(const QVariantMap &notificationHints);
};

#endif // NOTIFICATIONS_H
