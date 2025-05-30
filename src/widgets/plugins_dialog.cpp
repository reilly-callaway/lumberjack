#include "plugins_dialog.hpp"
#include "plugin_registry.hpp"


PluginsDialog::PluginsDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);

    initPluginsTable();

    // Add in each type of plugin interface
    ui.plugin_select->addItem(tr("Data Import"));
    ui.plugin_select->addItem(tr("Data Export"));
    ui.plugin_select->addItem(tr("Data Filter"));

    connect(ui.plugin_select, &QComboBox::currentIndexChanged, this, &PluginsDialog::selectPluginType);
    connect(ui.closeButton, &QPushButton::released, this, &PluginsDialog::close);

    // Display plugin locations
    QStringList paths = QCoreApplication::libraryPaths();

    ui.library_paths->setText(
        tr("Library Paths") + ":\n- " + paths.join("\n- ")
    );
}



void PluginsDialog::initPluginsTable(void)
{
    QStringList headers;

    headers << tr("Plugin Name");
    headers << tr("Description");
    headers << tr("Version");

    QTableWidget *table = ui.plugin_table;

    table->setColumnCount(headers.count());
    table->setHorizontalHeaderLabels(headers);

    table->verticalHeader()->setVisible(false);
}


void PluginsDialog::loadPluginsTable(const PluginList &plugins)
{
    QTableWidget *table = ui.plugin_table;

    table->setRowCount(0);

    for (auto plugin : plugins)
    {
        if (plugin.isNull()) continue;

        int row = table->rowCount();

        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(plugin->pluginName()));
        table->setItem(row, 1, new QTableWidgetItem(plugin->pluginDescription()));
        table->setItem(row, 2, new QTableWidgetItem(plugin->pluginVersion()));

        // TODO: Show where it was loaded from (file location)
    }
}


void PluginsDialog::selectPluginType(int idx)
{
    auto* registry = PluginRegistry::getInstance();

    PluginList plugins;

    switch (idx)
    {
    case 1:  // Importer plugins
        for (auto plugin : registry->ImportPlugins())
        {
            plugins.append(plugin);
        }
        break;
    case 2:  // Exporter plugins
        for (auto plugin : registry->ExportPlugins())
        {
            plugins.append(plugin);
        }
        break;
    case 3: // Filter plugins
        for (auto plugin : registry->FilterPlugins())
        {
            plugins.append(plugin);
        }
        break;
    default:
        break;
    }

    loadPluginsTable(plugins);
}

