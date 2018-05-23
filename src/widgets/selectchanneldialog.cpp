#include "selectchanneldialog.hpp"

#include "application.hpp"
#include "providers/twitch/twitchserver.hpp"
#include "util/layoutcreator.hpp"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <widgets/notebook.hpp>

#define TAB_TWITCH 0

namespace chatterino {
namespace widgets {

SelectChannelDialog::SelectChannelDialog()
    : BaseWindow((QWidget *)nullptr, BaseWindow::EnableCustomFrame)
    , selectedChannel(Channel::getEmpty())
{
    this->setWindowTitle("Select a channel to join");

    this->tabFilter.dialog = this;

    util::LayoutCreator<QWidget> layoutWidget(this->getLayoutContainer());
    auto layout = layoutWidget.setLayoutType<QVBoxLayout>().withoutMargin();
    auto notebook = layout.emplace<Notebook>(this).assign(&this->ui.notebook);

    // twitch
    {
        util::LayoutCreator<QWidget> obj(new QWidget());
        auto vbox = obj.setLayoutType<QVBoxLayout>();

        vbox.emplace<QLabel>("Enter the channel name below");
        auto lineEdit = vbox.emplace<QLineEdit>();
        vbox.emplace<QLabel>("or join a special channel...");

        auto hbox = vbox.emplace<QHBoxLayout>();
        {
            hbox.emplace<QPushButton>("Whispers");
            hbox.emplace<QPushButton>("Mentions");
            hbox.emplace<QPushButton>("Watching");
        }

        // tab
        NotebookTab *tab = notebook->addPage(obj.getElement());
        tab->setTitle("Twitch");
    }

    // irc
    /*{
        util::LayoutCreator<QWidget> obj(new QWidget());
        auto vbox = obj.setLayoutType<QVBoxLayout>();

        auto edit = vbox.emplace<QLabel>("not implemented");

        NotebookTab2 *tab = notebook->addPage(obj.getElement());
        tab->setTitle("Irc");
    }*/

    layout->setStretchFactor(*notebook, 1);

    auto buttons = layout.emplace<QHBoxLayout>().emplace<QDialogButtonBox>(this);
    {
        auto *button_ok = buttons->addButton(QDialogButtonBox::Ok);
        QObject::connect(button_ok, &QPushButton::clicked, [=](bool) { this->ok(); });
        auto *button_cancel = buttons->addButton(QDialogButtonBox::Cancel);
        QObject::connect(button_cancel, &QAbstractButton::clicked, [=](bool) { this->close(); });
    }

    this->setScaleIndependantSize(300, 210);

    // Shortcuts
    auto *shortcut_ok = new QShortcut(QKeySequence("Return"), this);
    QObject::connect(shortcut_ok, &QShortcut::activated, [=] { this->ok(); });
    auto *shortcut_cancel = new QShortcut(QKeySequence("Esc"), this);
    QObject::connect(shortcut_cancel, &QShortcut::activated, [=] { this->close(); });
}

void SelectChannelDialog::ok()
{
    this->_hasSelectedChannel = true;
    this->close();
}

void SelectChannelDialog::setSelectedChannel(IndirectChannel _channel)
{
    auto channel = _channel.get();

    assert(channel);

    this->selectedChannel = channel;

    //    switch (_channel.getType()) {
    //        case Channel::Twitch: {
    //            this->ui.notebook->selectIndex(TAB_TWITCH);
    //            this->ui.twitch.channel->setFocus();
    //            this->ui.twitch.channelName->setText(channel->name);
    //        } break;
    //        case Channel::TwitchWatching: {
    //            this->ui.notebook->selectIndex(TAB_TWITCH);
    //            this->ui.twitch.watching->setFocus();
    //        } break;
    //        case Channel::TwitchMentions: {
    //            this->ui.notebook->selectIndex(TAB_TWITCH);
    //            this->ui.twitch.mentions->setFocus();
    //        } break;
    //        case Channel::TwitchWhispers: {
    //            this->ui.notebook->selectIndex(TAB_TWITCH);
    //            this->ui.twitch.whispers->setFocus();
    //        } break;
    //        default: {
    //            this->ui.notebook->selectIndex(TAB_TWITCH);
    //            this->ui.twitch.channel->setFocus();
    //        }
    //    }

    this->_hasSelectedChannel = false;
}

IndirectChannel SelectChannelDialog::getSelectedChannel() const
{
    if (!this->_hasSelectedChannel) {
        return this->selectedChannel;
    }

    auto app = getApp();

    //    switch (this->ui.notebook->getSelectedIndex()) {
    //        case TAB_TWITCH: {
    //            if (this->ui.twitch.channel->isChecked()) {
    //                return
    //                app->twitch.server->getOrAddChannel(this->ui.twitch.channelName->text());
    //            } else if (this->ui.twitch.watching->isChecked()) {
    //                return app->twitch.server->watchingChannel;
    //            } else if (this->ui.twitch.mentions->isChecked()) {
    //                return app->twitch.server->mentionsChannel;
    //            } else if (this->ui.twitch.whispers->isChecked()) {
    //                return app->twitch.server->whispersChannel;
    //            }
    //        }
    //    }

    return this->selectedChannel;
}

bool SelectChannelDialog::hasSeletedChannel() const
{
    return this->_hasSelectedChannel;
}

bool SelectChannelDialog::EventFilter::eventFilter(QObject *watched, QEvent *event)
{
    //    auto *widget = (QWidget *)watched;

    //    if (event->type() == QEvent::FocusIn) {
    //        widget->grabKeyboard();

    //        auto *radio = dynamic_cast<QRadioButton *>(watched);
    //        if (radio) {
    //            radio->setChecked(true);
    //        }

    //        return true;
    //    } else if (event->type() == QEvent::FocusOut) {
    //        widget->releaseKeyboard();
    //        return false;
    //    } else if (event->type() == QEvent::KeyPress) {
    //        QKeyEvent *event_key = static_cast<QKeyEvent *>(event);
    //        if ((event_key->key() == Qt::Key_Tab || event_key->key() == Qt::Key_Down) &&
    //            event_key->modifiers() == Qt::NoModifier) {
    //            if (widget == this->dialog->ui.twitch.channelName) {
    //                this->dialog->ui.twitch.whispers->setFocus();
    //                return true;
    //            } else {
    //                widget->nextInFocusChain()->setFocus();
    //            }
    //            return true;
    //        } else if (((event_key->key() == Qt::Key_Tab || event_key->key() == Qt::Key_Backtab)
    //        &&
    //                    event_key->modifiers() == Qt::ShiftModifier) ||
    //                   ((event_key->key() == Qt::Key_Up) && event_key->modifiers() ==
    //                   Qt::NoModifier))
    //                   {
    //            if (widget == this->dialog->ui.twitch.channelName) {
    //                this->dialog->ui.twitch.watching->setFocus();
    //                return true;
    //            } else if (widget == this->dialog->ui.twitch.whispers) {
    //                this->dialog->ui.twitch.channel->setFocus();
    //                return true;
    //            }

    //            widget->previousInFocusChain()->setFocus();
    //            return true;
    //        } else {
    //            return false;
    //        }
    //        return true;
    //    } else if (event->type() == QEvent::KeyRelease) {
    //        QKeyEvent *event_key = static_cast<QKeyEvent *>(event);
    //        if ((event_key->key() == Qt::Key_Backtab || event_key->key() == Qt::Key_Down) &&
    //            event_key->modifiers() == Qt::NoModifier) {
    //            return true;
    //        }
    //    }

    return false;
}

void SelectChannelDialog::closeEvent(QCloseEvent *)
{
    this->closed.invoke();
}

void SelectChannelDialog::themeRefreshEvent()
{
    BaseWindow::themeRefreshEvent();

    if (this->themeManager->isLightTheme()) {
        this->setStyleSheet("QRadioButton { color: #000 } QLabel { color: #000 }");
    } else {
        this->setStyleSheet("QRadioButton { color: #fff } QLabel { color: #fff }");
    }
}

}  // namespace widgets
}  // namespace chatterino
