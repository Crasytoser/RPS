#include <pebble.h>
float timer_delay=1; /* ������� ���������� ��� � �������� ��� ������� */
Window *window; /* ������� ��������� �� ���� */
TextLayer *text_layer;  /* �������  ��������� �� ��������� ���� */
static BitmapLayer *image_layer; /* �������  ��������� �� ����������� ���� */
static GBitmap *image; /* �������  ��������� �� ����������� � ������ */
bool first_time=true; /* ������� ���� ������� ������� */
static const uint32_t images[] = {RESOURCE_ID_paper,RESOURCE_ID_rock,RESOURCE_ID_scissors,}; /* ������� ������ � �������� �������� */

void timer_call() /* ��� ������� ���������� ��� ������������� ������� � ��� ������ ������� �������� */
{
    if (first_time == false)/* ���� ����������� �� � ������ ���... */
    { 
        bitmap_layer_destroy(image_layer); /* ...�� ������� ������ ����, ����� �� �� ������� ������... */
        gbitmap_destroy(image); /* ...� ������� ������ �� ���������� �������� */
    }
    first_time = false; /* ���������� ���� ������� ������� */
    image_layer = bitmap_layer_create(GRect(32, 44, 80, 80));  /* ������� ���� � ������ ������ */
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(image_layer)); /* ������ ��� �������� */
    image = gbitmap_create_with_resource(images[rand() % 3]); /* ��������� � ������ ��������� �������� �� ������������ �������� */
    bitmap_layer_set_bitmap(image_layer, image); /* ������� ����������� �������� � ���� */
    light_enable_interaction(); /* �������� ��������� */
    if (timer_delay < 300*1000 ) /* ���� �������� ��� �� �������� 300��... */
    {
        timer_delay=timer_delay/0.7; /* ...����������� ��������... */
        app_timer_register(timer_delay/1000, timer_call, NULL); /* ...� ������� ������ ������ */
    }
    else /* ���� �������� ��� ������ 300��... */
    {
        timer_delay=1; /* ���������� ������ �� ������ � ������� - ��������� � �������� �� �� ��� �������� */
    }
}

void accel_int(AccelAxisType axis, int32_t direction) /* ��� ����������� ���������� �� �������������... */
{
    if (first_time == true) /* ���� ��� ������ ������... */
    { 
        text_layer_destroy(text_layer); /* ...�� ������� ��������� ���� � ����������� */
    }
    timer_call(); /* ��������� ������� */
}

int main(void)
{
    window = window_create();  /* �������������� ���� */
    window_set_background_color(window, GColorBlack); /* ������������� ������� ���� */
    window_set_fullscreen(window, true); /* �������� ��������������� */
    window_stack_push(window, true);  /* ��������� ���� � ��������� */
    srand(time(NULL)); /* �������������� ��������� ��������� ����� ������� �������� */
    text_layer = text_layer_create(GRect(0 , 30, 144, 168)); /* ������� ��������� ������, ��������� ������ � ���������� */
    text_layer_set_text_color(text_layer, GColorWhite);  /* ������������� ���� ������ */
    text_layer_set_background_color(text_layer, GColorClear);  /* ������������� ���� ���� */
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28)); /* ������������� ����� */
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter); /* ������������� ������������ �� ������ */
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));  /* ���������� ��������� ���� � ��������� � �������� ��������� */
    text_layer_set_text(text_layer, "���������� ����");  /* ���������� ��������� ��� ������� */
    accel_tap_service_subscribe(accel_int);  /* ������������� �� ���������� �� ������������� */
    app_event_loop();  /* ���� ������� */
    if (first_time == true) /* ���� ������� ��� ������� ��������... */
    { 
        text_layer_destroy(text_layer); /* ...�� ������� ��������� ���� � ���������� */
    }
    else  /* ���� ������� ��� ����� �������... */
    {    
        bitmap_layer_destroy(image_layer); /* ...�� ���������� ��������� ����... */
        gbitmap_destroy(image); /* ... � ���������� ������ � ��������, ��������� ���� ��� ������ � ������� accel_int */
    }
    accel_tap_service_unsubscribe();  /* ������������ �� ���������� ������������� */
    window_destroy(window);  /* ���������� ������� ����, ����������� ������� */
}
