/* © Sergey Roganov, 2017 */
/* © Сергей Роганов, 2017 */
/* GNU/GPL v.3+ */

(function() {
	var dom;

	dom = {
		title: null,
		header: null,
		menu: null,
		nav: null
	};

	window.addEventListener("load", function() {
		var i, j, len, ref, results;
		dom.title = document.querySelector("body>header>.title");
		dom.header = document.querySelector("body>main>header");
		dom.menu = document.querySelector("body>#menu");
		dom.nav = document.querySelector("body>nav");
		dom.title.style.opacity = 0;
		ref = dom.nav.children;
		results = [];
		for (j = 0, len = ref.length; j < len; j++) {
			i = ref[j];
			results.push(i.addEventListener("click", function(ev) {
				var base;
				return dom.menu.checked = typeof (base = dom.menu).checked === "function" ? base.checked({
					"false": true
				}) : void 0;
			}));
		}
		return results;
	});

	window.addEventListener("scroll", function(ev) {
		if ((document.documentElement.scrollTop || document.body.scrollTop || window.scrollY) > parseInt(window.getComputedStyle(dom.header).height) * .5) {
			return dom.title.style.opacity = 1;
		} else {
			return dom.title.style.opacity = 0;
		}
	});

}).call(this);
