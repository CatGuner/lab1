#include <gst/gst.h>

int	main(int ac, char **av)
{
	GstElement	*source, *pipeline;
	GstBus		*bus;
	GstMessage	*msg;
	GstStateChangeReturn	ret;

	gst_init(&ac, &av);

	source = gst_element_factory_make ("playbin", "source");

	pipeline = gst_pipeline_new ("test-pipeline");

	if (!pipeline || !source)
	{
		g_printerr("Not all elements could be created.\n");
		return -1;
	}

	if (!(gst_bin_add (GST_BIN (pipeline), source)))
	{
		g_printerr ("Unnable to set the pipeline to the playing state.\n");
		gst_object_unref (pipeline);
		return -1;
	}

	g_object_set (source, "uri", 
			"http://techslides.com/demos/sample-videos/small.webm", 
			NULL);

	ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref (pipeline);
		return -1;
	}


	bus = gst_element_get_bus(pipeline);
	msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
			GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
	if (msg != NULL)
		gst_message_unref(msg);
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	return 0;
}
