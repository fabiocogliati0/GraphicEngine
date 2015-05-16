/*#include "window.h"

#include <windows.h>
#include <d3d11.h>

namespace graphicengine
{

	window::Window(
		const hinstance hinstance,
		int ncmdshow,
		int multisamplecount,
		const lpcwstr title,
		long width,
		long height)
	{
		initialize window descriptor
		m_window.cbsize = sizeof(wndclassex);
		m_window.style = cs_hredraw | cs_vredraw;
		m_window.lpfnwndproc = defaultwndproc;
		m_window.cbclsextra = 0;
		m_window.cbwndextra = 0;
		m_window.hinstance = hinstance;
		m_window.hicon = loadicon(hinstance, (lpctstr)idi_application);
		m_window.hcursor = loadcursor(nullptr, idc_arrow);
		m_window.hbrbackground = (hbrush)(color_window + 1);
		m_window.lpszmenuname = nullptr;
		m_window.lpszclassname = l"graphicsprogrammingwindow";
		m_window.hiconsm = loadicon(m_window.hinstance, (lpctstr)idi_application);

		initialize window rect
		m_windowrect.left = 0;
		m_windowrect.top = 0;
		m_windowrect.right = width;
		m_windowrect.bottom = height;

		set other properties
		m_swindowtitle = title;
		m_ncmdshow = ncmdshow;
		m_muntisamplecount = multisamplecount;
	}

	window::~window()
	{

	}

	int window::run()
	{
		if (failed(initializewindow()))
			return 1;

		if (failed(initializedirectx()))
			return 1;

		if (failed(initializeresources()))
			return 1;

		mrunningwindow = this;

		 main message loop
		msg msg = { 0 };
		while (wm_quit != msg.message)
		{
			if (peekmessage(&msg, nullptr, 0, 0, pm_remove))
			{
				translatemessage(&msg);
				dispatchmessage(&msg);
			}
			else
			{
				renderwindowcontent();
			}
		}

		mrunningwindow = nullptr;
		return (int)msg.wparam;
	}

	hresult window::initializewindow()
	{
		registry class
		if (!registerclassex(&m_window))
		{
			return e_fail;
		}

		window creation
		adjustwindowrect(&m_windowrect, ws_overlappedwindow, false);

		m_windowhandler = createwindow
			(
			l"graphicsprogrammingwindow",
			m_swindowtitle,
			ws_overlappedwindow,
			cw_usedefault,
			cw_usedefault,
			m_windowrect.right - m_windowrect.left,
			m_windowrect.bottom - m_windowrect.top,
			nullptr,
			nullptr,
			m_window.hinstance,
			nullptr
			);

		if (!m_windowhandler)
		{
			return e_fail;
		}

		showwindow(m_windowhandler, m_ncmdshow);

		return s_ok;
	}

	hresult window::initializedirectx()
	{
		hresult hr = s_ok;

		 registra l'area in cui compare la finestra
		rect rc;
		getclientrect(m_windowhandler, &rc);
		uint width = rc.right - rc.left;
		uint height = rc.bottom - rc.top;

		hr = initializedevice();
		if (failed(hr))
			return e_fail;

		hr = initializerendertarget();
		if (failed(hr))
			return e_fail;

		 setta il viewport
		d3d11_viewport _viewport;
		_viewport.width = static_cast<float>(width);
		_viewport.height = static_cast<float>(height);
		_viewport.mindepth = 0.0f;
		_viewport.maxdepth = 1.0f;
		_viewport.topleftx = 0;
		_viewport.toplefty = 0;
		m_devicecontext->rssetviewports(1, &_viewport);

		return s_ok;
	}

	hresult window::initializedevice()
	{
		hresult hr = s_ok;

		 registra l'area in cui compare la finestra
		rect rc;
		getclientrect(m_windowhandler, &rc);
		uint width = rc.right - rc.left;
		uint height = rc.bottom - rc.top;

		uint createdeviceflags = 0;
#ifdef _debug
		 crea un device in modalità debug
		createdeviceflags |= d3d11_create_device_debug;
#endif

		 lo swap chain è responsabile del rendering a schermo effettivo. 
		 solitamente è inizializzato in modalità double buffered, ovvero contiene solitamente due buffer: front e back
		dxgi_swap_chain_desc sd;
		zeromemory(&sd, sizeof(sd));
		 numero di buffer
		sd.buffercount = 1; // numero di back_buffer
		sd.bufferdesc.width = width;
		sd.bufferdesc.height = height;
		sd.bufferdesc.format = dxgi_format_r8g8b8a8_unorm; // formato rgba a 32bit
		sd.bufferdesc.refreshrate.numerator = 60; // refresh rate schermo
		sd.bufferdesc.refreshrate.denominator = 1; // fattore di divisione refresh rate (intero)
		sd.bufferusage = dxgi_usage_render_target_output; // utilizzo del backbuffer: il rendering viene scritto qui
		sd.outputwindow = m_windowhandler; // handler finistra
		sd.sampledesc.count = m_muntisamplecount; // serve per il multisampling - 1 viene disabilitato
		sd.sampledesc.quality = 0;
		sd.windowed = true; // modalità fullscree/windows

		 definisce quali tipi di driver compatibili cercare
		 hardware: utilizza schede video. reference: se il driver "emula" in modo software alcune funzioni. (_null : driver incapace di effettuare il rendering)
		d3d_driver_type drivertypes[] =
		{
			d3d_driver_type_hardware,
			d3d_driver_type_reference,
			d3d_driver_type_software,
		};
		uint numdrivertypes = sizeof(drivertypes) / sizeof(drivertypes[0]);

		for (uint drivertypeindex = 0; drivertypeindex < numdrivertypes; ++drivertypeindex)
		{
			 prova ad inizializzare per i tipi di driver definiti
			m_drivertype = drivertypes[drivertypeindex];
			hr = d3d11createdeviceandswapchain(nullptr, m_drivertype, nullptr, createdeviceflags, nullptr, 0,
				d3d11_sdk_version, &sd, &m_swapchain, &m_device, nullptr, &m_devicecontext);
			 appena funziona esci
			if (succeeded(hr))
			{
				if (m_drivertype != d3d_driver_type_hardware)
				break;
			}
		}

		 errore: niente di buono qui.
		if (failed(hr))
		{
			return hr;
		}

		return s_ok;

	}

	hresult window::initializerendertarget()
	{
		hresult hr = s_ok;

		rect rc;
		getclientrect(m_windowhandler, &rc);
		uint width = rc.right - rc.left;
		uint height = rc.bottom - rc.top;

		 ottieni il backbuffer. il back buffer è semplicemente una texture 2d. una texture 2d è semplicemente una matrice bidimensionale di valori.
		id3d11texture2d* pbackbuffer;
		hr = m_swapchain->getbuffer(0, __uuidof(id3d11texture2d), (lpvoid*)&pbackbuffer);
		if (failed(hr))
		{
			return hr;
		}

		 crea il render target e collega l'output alla texture 2d
		hr = m_device->createrendertargetview(pbackbuffer, nullptr, &m_rendertargetview);
		pbackbuffer->release();
		if (failed(hr))
		{
			return hr;
		}

		 creazione del depth e stencil buffer.
		id3d11texture2d* pdepthstencil = nullptr;
		d3d11_texture2d_desc descdepth;
		descdepth.width = width;
		descdepth.height = height;
		descdepth.miplevels = 1;
		descdepth.arraysize = 1;
		descdepth.format = dxgi_format_d24_unorm_s8_uint;;
		descdepth.sampledesc.count = m_muntisamplecount;
		descdepth.sampledesc.quality = 0;
		descdepth.usage = d3d11_usage_default;
		descdepth.bindflags = d3d11_bind_depth_stencil;
		descdepth.cpuaccessflags = 0;
		descdepth.miscflags = 0;

		hr = m_device->createtexture2d(&descdepth, nullptr, &pdepthstencil);
		if (failed(hr))
		{
			return hr;
		}

		d3d11_depth_stencil_view_desc descdsv;
		descdsv.format = dxgi_format_d24_unorm_s8_uint;
		descdsv.viewdimension = (m_muntisamplecount > 1) ? d3d11_dsv_dimension_texture2dms : d3d11_dsv_dimension_texture2d;
		descdsv.texture2d.mipslice = 0;
		descdsv.flags = 0;

		 create the depth stencil view
		hr = m_device->createdepthstencilview(pdepthstencil, &descdsv, &m_depthstencilview);
		if (failed(hr))
		{
			return hr;
		}

		 collega il buffer al merger stage in modo che l'output sia effettuato qui
		m_devicecontext->omsetrendertargets(1, &m_rendertargetview, m_depthstencilview);

		return s_ok;
	}

	lresult callback defaultwndproc(hwnd hwnd, uint message, wparam wparam, lparam lparam)
	{
		paintstruct ps;
		hdc hdc;

		switch (message)
		{
		case wm_paint:
		{
						  chiamato ogni volta si necessita di ridisegnare qualcosa a schermo
						 hdc = beginpaint(hwnd, &ps);
						 endpaint(hwnd, &ps);
		} break;

		case wm_destroy:
		{
						    chiamato prima di uscire dall'applicazione
						   postquitmessage(0);
		} break;

		default:
		{
				    per tutti gli altri eventi, lascia a windows gestire le chiamate
				   return defwindowproc(hwnd, message, wparam, lparam);
		}
		}

		return 0;
	}

}*/