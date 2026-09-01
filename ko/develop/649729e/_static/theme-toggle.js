(function () {
    'use strict';

    const CFG = {
        minW: 98, iconW: 24, maxW: 360, defW: 240,
        storageKey: 'sidebarGridWidth',
        hideDelay: 2000
    };

    if (window.sidebarControlInstance) {
        window.sidebarControlInstance.destroy();
    }

    class SidebarSystem {
        constructor() {
            const saved = parseInt(localStorage.getItem(CFG.storageKey), 10);
            this.state = {
                curW: Number.isFinite(saved) ? saved : CFG.defW,
                isDragging: false
            };
            this.dom = {};
            this.ticking = { main: false, drag: false };
            this.timer = null;
            this.ro = null;

            this.boundResize = () => this.updateAll();
            this.boundScroll = () => this.handleScroll();
            this.boundMove = (e) => this.onMouseMove(e);
            this.boundUp = () => this.onMouseUp();

            this.init();
        }

        init() {
            if (!this.cacheDOM()) return;
            this.injectStyles();
            this.buildUI();
            this.attachEvents();
            this.updateAll();
        }

        cacheDOM() {
            this.dom.sbL = document.getElementById('left-sidebar');
            this.dom.sbR = document.getElementById('right-sidebar');
            this.dom.main = document.querySelector('main');
            this.dom.cntr = this.dom.sbL?.parentElement;
            return !!(this.dom.sbL && this.dom.sbR && this.dom.cntr);
        }

        injectStyles() {
            if (document.getElementById('sb-final-fixed-v2')) return;
            const style = document.createElement('style');
            style.id = 'sb-final-fixed-v2';
            style.textContent = `
                :root { --sb-l-w: ${CFG.defW}px; --sb-gap: calc(var(--spacing) * 9); }
                @media (min-width: 1024px) { :root { --sb-gap: calc(var(--spacing) * 13); } }
                #sidebar-grid-resizer { position: absolute; left: calc(var(--sb-gap) * -1 + 2px); top: 0; width: 8px; height: 100%; cursor: ew-resize; z-index: 1000; display: none; touch-action: none; }
                #sidebar-expand-icon { position: fixed; top: 40%; left: 2%; z-index: 1001; cursor: pointer; display: none; background: var(--color-muted); color: var(--color-muted-foreground); padding: 6px; border: 1px solid var(--color-border); border-radius: 4px; align-items: center; justify-content: center; }
                #scrolltop-toggle { position: fixed; top: 87px; right: 1.9%; z-index: 9999; background: var(--color-muted); color: var(--color-muted-foreground); border: 1px solid var(--color-border); border-radius: 4px; padding: 6px; cursor: pointer; display: none; align-items: center; justify-content: center; transition: opacity .5s ease, transform 0.3s ease; opacity: 1; }
                .icon-mode { overflow: hidden; }
                .icon-mode > *:not(#sidebar-expand-icon) { opacity: 0; pointer-events: none; }
                .resizing { cursor: ew-resize !important; user-select: none !important; }
                .force-hide-sidebar { display: none !important; }
                .full-width-layout { grid-template-columns: 1fr !important; }
            `;
            document.head.appendChild(style);
        }

        buildUI() {
            this.dom.resizer = document.createElement('div');
            this.dom.resizer.id = 'sidebar-grid-resizer';
            this.dom.exBtn = document.createElement('button');
            this.dom.exBtn.id = 'sidebar-expand-icon';
            this.dom.tgBtn = document.createElement('button');
            this.dom.tgBtn.id = 'scrolltop-toggle';

            this.dom.exBtn.innerHTML = `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9 18l6-6-6-6"/></svg>`;
            this.dom.tgBtn.innerHTML = `<svg height="18" viewBox="0 0 24 24" width="18" fill="currentColor" style="transition: transform 0.3s"><path d="M10 6L8.59 7.41 13.17 12l-4.58 4.59L10 18l6-6z"/></svg>`;

            const wrap = this.dom.sbL.nextElementSibling;
            if (wrap) {
                wrap.style.position = 'relative';
                wrap.insertBefore(this.dom.resizer, wrap.firstChild);
            }
            this.dom.resizer.appendChild(this.dom.exBtn);
            document.body.appendChild(this.dom.tgBtn);
        }

        attachEvents() {
            this.dom.resizer.addEventListener('mousedown', (e) => this.onDragStart(e));
            window.addEventListener('resize', this.boundResize);
            window.addEventListener('scroll', this.boundScroll, { passive: true });
            
            this.dom.tgBtn.addEventListener('click', () => this.onToggleRight());
            this.dom.exBtn.addEventListener('click', () => this.onExpandLeft());
            
            this.dom.tgBtn.addEventListener('mouseenter', () => this.clearTimer());
            this.dom.tgBtn.addEventListener('mouseleave', () => {
                if (this.dom.sbR.dataset.manualHidden === "true") this.resetTimer();
            });
            
            this.ro = new ResizeObserver(() => this.updateAll());
            this.ro.observe(this.dom.sbL);
            this.ro.observe(this.dom.sbR);
        }

        isVisible(el, side) {
            if (!el || el.offsetWidth <= 0) return false;
            const rect = el.getBoundingClientRect();
            return side === 'left' ? rect.right > 5 : rect.left < window.innerWidth - 5;
        }

        updateLeft(w = this.state.curW) {
            const show = this.isVisible(this.dom.sbL, 'left');
            this.dom.resizer.style.display = show ? 'block' : 'none';
            if (show) {
                const isIcon = w <= CFG.minW;
                const finalW = isIcon ? CFG.iconW : w;
                this.dom.cntr.style.gridTemplateColumns = `${finalW}px minmax(0, 1fr)`;
                this.dom.sbL.classList.toggle('icon-mode', isIcon);
                this.dom.exBtn.style.display = isIcon ? 'flex' : 'none';
            }
        }

        updateRight() {
            this.dom.sbR.classList.remove('force-hide-sidebar');
            const show = this.isVisible(this.dom.sbR, 'right');
            const manual = this.dom.sbR.dataset.manualHidden === "true";

            if (!show) {
                this.dom.tgBtn.style.display = 'none';
                this.dom.main.classList.remove('full-width-layout');
                this.clearTimer();
            } else {
                this.dom.tgBtn.style.display = 'flex';
                this.dom.tgBtn.firstElementChild.style.transform = manual ? "rotate(180deg)" : "rotate(0deg)";
                
                if (manual) {
                    this.dom.sbR.classList.add('force-hide-sidebar');
                    this.dom.main.classList.add('full-width-layout');
                    this.resetTimer();
                } else {
                    this.dom.sbR.classList.remove('force-hide-sidebar');
                    this.dom.main.classList.remove('full-width-layout');
                    this.clearTimer();
                    this.dom.tgBtn.style.opacity = "1";
                }
            }
        }

        updateAll() {
            if (this.ticking.main) return;
            this.ticking.main = true;
            requestAnimationFrame(() => {
                this.updateLeft();
                this.updateRight();
                this.ticking.main = false;
            });
        }

        onDragStart(e) {
            this.state.isDragging = true;
            this.state.cLeft = this.dom.cntr.getBoundingClientRect().left;
            document.body.classList.add('resizing');
            window.addEventListener('mousemove', this.boundMove);
            window.addEventListener('mouseup', this.boundUp);
        }

        onMouseMove(ev) {
            if (!this.state.isDragging || this.ticking.drag) return;
            this.ticking.drag = true;
            requestAnimationFrame(() => {
                this.state.curW = Math.max(CFG.iconW, Math.min(CFG.maxW, ev.clientX - this.state.cLeft));
                this.updateLeft();
                this.ticking.drag = false;
            });
        }

        onMouseUp() {
            this.state.isDragging = false;
            document.body.classList.remove('resizing');
            localStorage.setItem(CFG.storageKey, this.state.curW);
            window.removeEventListener('mousemove', this.boundMove);
            window.removeEventListener('mouseup', this.boundUp);
        }

        onToggleRight() {
            const isHidden = this.dom.sbR.dataset.manualHidden === "true";
            this.dom.sbR.dataset.manualHidden = String(!isHidden);
            this.updateRight();
        }

        onExpandLeft() {
            this.state.curW = CFG.defW;
            this.updateLeft();
            localStorage.setItem(CFG.storageKey, this.state.curW);
        }

        handleScroll() {
            if (this.dom.sbR.dataset.manualHidden === "true") {
                this.resetTimer();
            }
        }

        resetTimer() {
            if (this.dom.sbR.dataset.manualHidden !== "true") {
                this.clearTimer();
                this.dom.tgBtn.style.opacity = "1";
                return;
            }
            this.clearTimer();
            this.dom.tgBtn.style.opacity = "1";
            this.timer = setTimeout(() => {
                this.dom.tgBtn.style.opacity = "0.1";
            }, CFG.hideDelay);
        }

        clearTimer() {
            clearTimeout(this.timer);
            this.timer = null;
        }

        destroy() {
            this.ro?.disconnect();
            window.removeEventListener('resize', this.boundResize);
            window.removeEventListener('scroll', this.boundScroll);
            window.removeEventListener('mousemove', this.boundMove);
            window.removeEventListener('mouseup', this.boundUp);
            this.clearTimer();
            this.dom.resizer?.remove();
            this.dom.tgBtn?.remove();
            window.sidebarControlInstance = null;
        }
    }

    if (window.sidebarControlInstance) window.sidebarControlInstance.destroy();
    if (document.getElementById('left-sidebar')) {
        window.sidebarControlInstance = new SidebarSystem();
    }
})();
