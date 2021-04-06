#include "main.h"



static const int SETTINGS_MAGIC_NUMBER 		= 0x4744;


AppSettingsStruct_t settings;


bool loadConfigFile(void)
{
	g_autoptr(GError) error = NULL;
	g_autoptr(GKeyFile) key_file = g_key_file_new ();

	const char *path;
	char *file;

	path = g_get_current_dir();
	file = g_build_filename(path, "DMRStation.ini", NULL);
	g_free((gpointer)path);

	if (!g_key_file_load_from_file (key_file, file, G_KEY_FILE_NONE, &error))
    {
		if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
			g_warning ("Error loading key file: %s", error->message);
		return false;
    }

	g_autofree gchar *val;
	//user data
	val = g_key_file_get_string (key_file, "user", "callsign", &error);
	if (val != NULL)
	{
		g_snprintf((gchar *) settings.callsign, CALLSIGN_LENGTH, "%s", val);
	}

	val = g_key_file_get_string (key_file, "user", "dmrId", &error);
	if (val != NULL)
	{
		settings.dmrId = atoi(val);
	}

	val = g_key_file_get_string (key_file, "user", "repeaterId", &error);
	if (val != NULL)
	{
		settings.repeaterId = atoi(val);
	}

	val = g_key_file_get_string (key_file, "user", "initialTG", &error);
	if (val != NULL)
	{
		settings.initialTG = atoi(val);
	}

	//server data
	val = g_key_file_get_string (key_file, "server", "remoteHost", &error);
	if (val != NULL)
	{
		g_snprintf((gchar *) settings.remoteHost, HOST_LENGTH, "%s", val);
	}

	val = g_key_file_get_string (key_file, "server", "remotePort", &error);
	if (val != NULL)
	{
		settings.remotePort = atoi(val);
	}

	val = g_key_file_get_string (key_file, "server", "password", &error);
	if (val != NULL)
	{
		g_snprintf((gchar *) settings.password, HOST_LENGTH, "%s", val);
	}

	//ambe data
	val = g_key_file_get_string (key_file, "ambe", "ambeServerHost", &error);
	if (val != NULL)
	{
		g_snprintf((gchar *) settings.ambeServerHost, HOST_LENGTH, "%s", val);
	}

	val = g_key_file_get_string (key_file, "ambe", "ambeServerPort", &error);
	if (val != NULL)
	{
		settings.ambeServerPort = atoi(val);
	}

	//codec2 data
	val = g_key_file_get_string (key_file, "codec2", "codec2ServerHost", &error);
	if (val != NULL)
	{
		g_snprintf((gchar *) settings.codec2ServerHost, HOST_LENGTH, "%s", val);
	}

	val = g_key_file_get_string (key_file, "codec2", "codec2ServerPort", &error);
	if (val != NULL)
	{
		settings.codec2ServerPort = atoi(val);
	}

	val = g_key_file_get_string (key_file, "codec2", "enabled", &error);
	if (val != NULL)
	{
		settings.codec2Enabled = atoi(val);
	}

	val = g_key_file_get_string (key_file, "ui", "smallUI", &error);
	if (val != NULL)
	{
		settings.smallUI = atoi(val);
	}

	val = g_key_file_get_string (key_file, "ui", "decorated", &error);
	if (val != NULL)
	{
		settings.decorated = atoi(val);
	}

	//misc options
	val = g_key_file_get_string (key_file, "misc", "tot", &error);
	if (val != NULL)
	{
		settings.tot = atoi(val);
	}
	val = g_key_file_get_string (key_file, "misc", "totReverse", &error);
	if (val != NULL)
	{
		settings.totReverse = atoi(val);
	}

	//ptt
	val = g_key_file_get_string (key_file, "ptt", "pttBank", &error);
	if (val != NULL)
	{
		g_snprintf((gchar *) settings.pttBank, PTT_BANK_LENGTH, "%s", val);
	}

	val = g_key_file_get_string (key_file, "ptt", "pttPort", &error);
	if (val != NULL)
	{
		settings.pttPort = atoi(val);
	}

	val = g_key_file_get_string (key_file, "ptt", "pttEnabled", &error);
	if (val != NULL)
	{
		settings.pttEnabled = atoi(val);
	}

	val = g_key_file_get_string (key_file, "ptt", "pttInvert", &error);
	if (val != NULL)
	{
		settings.pttInvert = atoi(val);
	}

	//gpio

	val = g_key_file_get_string (key_file, "gpio", "gpioBank", &error);
	if (val != NULL)
	{
		g_snprintf((gchar *) settings.gpioBank, GPIO_BANK_LENGTH, "%s", val);
	}

	val = g_key_file_get_string (key_file, "gpio", "gpioConnectedPort", &error);
	if (val != NULL)
	{
		settings.gpioConnectedPort = atoi(val);
	}

	val = g_key_file_get_string (key_file, "gpio", "gpioEnabled", &error);
	if (val != NULL)
	{
		settings.gpioEnabled = atoi(val);
	}


return true;
}

/**
 * Load configuration or set some default settings
 */
void setDefaultSettings(void)
{
	//some default values
	settings.tot = 180;
	settings.totReverse = 0;
	settings.pttEnabled = 0;
	settings.pttInvert = 0;
	settings.currentTGPrivate = 0;
	settings.decorated = 0;
	settings.gpioEnabled = 0;

	if(loadConfigFile())
	{
		return;
	}

	//Default config
	settings.magicNumber = SETTINGS_MAGIC_NUMBER;
	settings.dmrId = 123456;
	settings.repeaterId = 123456788;
	settings.remotePort = 62031;
	settings.initialTG = 91;


	//local
	strcpy((char *) settings.remoteHost, (char *) "192.168.3.254");

	//My server on the net
	//strcpy((char *) settings.remoteHost, (char *) "87.98.228.225");

	//Master spain
	//strcpy((char *) settings.remoteHost, (char *) "84.232.5.113");

	strcpy((char *) settings.password, (char *) "passw0rd");
	strcpy((char *) settings.callsign, (char *) "EA3IHI");

	strcpy((char *) settings.ambeServerHost, (char *) "127.0.0.1");
	settings.ambeServerPort = 2460;

	strcpy((char *) settings.codec2ServerHost, (char *) "127.0.0.1");
	settings.codec2ServerPort = 2470;

}
