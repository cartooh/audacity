/*
* Audacity: A Digital Audio Editor
*/
import QtQuick
import QtQuick.Layouts

import Muse.Ui
import Muse.UiComponents

import Audacity.Import

StyledDialogView {
    id: root

    property string path: ""

    title: qsTrc("importexport", "Import raw data")

    contentWidth: 440
    contentHeight: 340

    margins: 16

    ImportRawModel {
        id: importModel
    }

    Component.onCompleted: {
        importModel.init(root.path)
    }

    QtObject {
        id: prv

        property int labelColumnWidth: 140
        property int controlColumnWidth: 240
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        StyledTextLabel {
            Layout.fillWidth: true

            text: importModel.fileName
            font: ui.theme.bodyBoldFont
            horizontalAlignment: Text.AlignLeft
        }

        GridLayout {
            Layout.fillWidth: true

            columns: 2
            columnSpacing: 12
            rowSpacing: 12

            StyledTextLabel {
                Layout.preferredWidth: prv.labelColumnWidth
                text: qsTrc("importexport", "Encoding:")
                horizontalAlignment: Text.AlignLeft
            }

            StyledDropdown {
                id: encodingDropdown

                Layout.preferredWidth: prv.controlColumnWidth

                model: importModel.encodingList
                currentIndex: importModel.currentEncodingIndex

                navigation.name: "EncodingDropdown"
                navigation.panel: navPanel
                navigation.row: 1

                onActivated: function (index, value) {
                    importModel.currentEncodingIndex = index
                }
            }

            StyledTextLabel {
                Layout.preferredWidth: prv.labelColumnWidth
                text: qsTrc("importexport", "Byte order:")
                horizontalAlignment: Text.AlignLeft
            }

            StyledDropdown {
                id: endianDropdown

                Layout.preferredWidth: prv.controlColumnWidth

                model: importModel.endianList
                currentIndex: importModel.currentEndianIndex

                navigation.name: "ByteOrderDropdown"
                navigation.panel: navPanel
                navigation.row: 2

                onActivated: function (index, value) {
                    importModel.currentEndianIndex = index
                }
            }

            StyledTextLabel {
                Layout.preferredWidth: prv.labelColumnWidth
                text: qsTrc("importexport", "Channels:")
                horizontalAlignment: Text.AlignLeft
            }

            StyledDropdown {
                id: channelsDropdown

                Layout.preferredWidth: prv.controlColumnWidth

                model: importModel.channelsList
                currentIndex: importModel.currentChannelsIndex

                navigation.name: "ChannelsDropdown"
                navigation.panel: navPanel
                navigation.row: 3

                onActivated: function (index, value) {
                    importModel.currentChannelsIndex = index
                }
            }

            StyledTextLabel {
                Layout.preferredWidth: prv.labelColumnWidth
                text: qsTrc("importexport", "Start offset (bytes):")
                horizontalAlignment: Text.AlignLeft
            }

            TextInputField {
                id: offsetField

                Layout.preferredWidth: prv.controlColumnWidth

                currentText: importModel.offset

                validator: IntValidator {
                    bottom: 0
                }

                navigation.name: "StartOffsetField"
                navigation.panel: navPanel
                navigation.row: 4

                onTextChanged: function (newTextValue) {
                    importModel.offset = newTextValue
                }
            }

            StyledTextLabel {
                Layout.preferredWidth: prv.labelColumnWidth
                text: qsTrc("importexport", "Amount to import (%):")
                horizontalAlignment: Text.AlignLeft
            }

            TextInputField {
                id: percentField

                Layout.preferredWidth: prv.controlColumnWidth

                currentText: importModel.percent

                validator: DoubleValidator {
                    bottom: 0.0
                    top: 100.0
                    decimals: 2
                }

                navigation.name: "AmountToImportField"
                navigation.panel: navPanel
                navigation.row: 5

                onTextChanged: function (newTextValue) {
                    importModel.percent = newTextValue
                }
            }

            StyledTextLabel {
                Layout.preferredWidth: prv.labelColumnWidth
                text: qsTrc("importexport", "Sample rate (Hz):")
                horizontalAlignment: Text.AlignLeft
            }

            StyledDropdown {
                id: sampleRateDropdown

                Layout.preferredWidth: prv.controlColumnWidth

                model: importModel.sampleRateList
                currentIndex: importModel.currentSampleRateIndex

                navigation.name: "SampleRateDropdown"
                navigation.panel: navPanel
                navigation.row: 6

                onActivated: function (index, value) {
                    importModel.currentSampleRateIndex = index
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        ButtonBox {
            id: buttonBox

            Layout.fillWidth: true

            navigationPanel.section: root.navigationSection
            navigationPanel.order: 2

            FlatButton {
                text: qsTrc("global", "Cancel")
                buttonRole: ButtonBoxModel.RejectRole
                buttonId: ButtonBoxModel.Cancel
                minWidth: 80

                onClicked: {
                    root.reject()
                }
            }

            FlatButton {
                text: qsTrc("importexport", "Import")
                buttonRole: ButtonBoxModel.AcceptRole
                buttonId: ButtonBoxModel.Apply
                minWidth: 80
                accentButton: true

                enabled: importModel.isValidFormat

                onClicked: {
                    if (importModel.doImport()) {
                        root.ret = {
                            errcode: 0
                        }
                        root.hide()
                    } else {
                        root.reject()
                    }
                }
            }
        }
    }

    property NavigationPanel navPanel: NavigationPanel {
        name: "ImportRawPanel"
        section: root.navigationSection
        direction: NavigationPanel.Vertical
        order: 1
    }
}
