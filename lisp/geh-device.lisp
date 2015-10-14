(clear-all)

(defmethod device-move-cursor-to ((device virtual-window) loc)
  (format t "%%geh-send-event[mouse-move,~A]%%" loc))

(defmethod device-handle-click ((device virtual-window))
  (format t "%%geh-send-event[mouse-click]%%"))

(defmethod device-handle-keypress ((device virtual-window) key)
   (format t "%%geh-send-event[key-press,~A]%%" key))

(defclass geh-widget (virtual-view) ())

(defmethod build-vis-locs-for ((widget geh-widget) vis-mod)
	(let ((x (car (define-chunks-fct `((isa visual-location
							   screen-x ,(x-pos widget)
							   screen-y ,(y-pos widget)
							   width ,(width widget)
							   height ,(height widget)))))))

		(setf (chunk-visual-object x) widget)
		x))

(defmethod vis-loc-to-obj ((widget geh-widget) vis-mod)
	(car (define-chunks-fct `((isa visual-object
							   screen-pos (isa visual-location
										   screen-x ,(x-pos widget)
										   screen-y ,(y-pos widget)))))))
