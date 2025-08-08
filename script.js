// script.js — interactions, progress bar, floating blobs, confetti
(function(){
  /* ==== Floating decorative blobs (lightweight) ==== */
  function createBlobs() {
    const wrap = document.createElement('div');
    wrap.className = 'background-lights';
    wrap.innerHTML = `
      <div class="light p1"></div>
      <div class="light p2"></div>
      <div class="light p3"></div>
    `;
    document.body.appendChild(wrap);
    // unobtrusive grid
    const grid = document.createElement('div'); grid.className = 'grid';
    document.body.appendChild(grid);
  }

  /* ==== Option selection & card animations ==== */
  function wireOptions() {
    document.querySelectorAll('.question-card').forEach((card, idx) => {
      // animate stagger if desired (small stagger)
      card.style.animationDelay = (idx * 60) + 'ms';
      card.querySelectorAll('.option').forEach(option => {
        option.addEventListener('click', () => {
          // toggle selection within this card
          card.querySelectorAll('.option').forEach(o => o.classList.remove('selected'));
          option.classList.add('selected');
          const input = option.querySelector('input[type="radio"]');
          if (input) input.checked = true;
          updateProgress();
        });
      });
    });
  }

  /* ==== Progress bar that updates when answers picked ==== */
  function updateProgress() {
    const qCards = document.querySelectorAll('.question-card');
    if (qCards.length === 0) return;
    let answered = 0;
    qCards.forEach((qc, i) => {
      const name = 'q' + i;
      if (qc.querySelector(`input[name="${name}"]:checked`)) answered++;
    });
    const pct = Math.round((answered / qCards.length) * 100);
    const fill = document.getElementById('progress-fill');
    if (fill) fill.style.width = pct + '%';
    // small pulse when completed
    const submit = document.querySelector('.btn[type="submit"]');
    if (submit) {
      if (pct === 100) {
        submit.classList.add('completed');
      } else submit.classList.remove('completed');
    }
  }

  /* ==== Form validation: prevent partial submit ==== */
  function wireFormValidation() {
    const quizForm = document.getElementById('quiz-form');
    if (!quizForm) return;
    quizForm.addEventListener('submit', e => {
      const qCards = document.querySelectorAll('.question-card');
      let answered = 0;
      qCards.forEach((qc, idx) => {
        if (quizForm.querySelector(`input[name="q${idx}"]:checked`)) answered++;
      });
      if (answered < qCards.length) {
        e.preventDefault();
        // small shake animation
        qCards.forEach((qc, i) => {
          if (!qc.querySelector(`input[name="q${i}"]:checked`)) {
            qc.animate([{ transform: 'translateX(0)' }, { transform: 'translateX(-6px)' }, { transform: 'translateX(6px)' }, { transform: 'translateX(0)' }], { duration: 360, easing: 'ease-out' });
          }
        });
        setTimeout(()=> alert(`Please answer all ${qCards.length} questions before submitting.`), 120);
      }
    });
  }

  /* ==== IntersectionObserver to reveal cards on scroll (performance friendly) ==== */
  function revealOnScroll() {
    const io = new IntersectionObserver((entries)=> {
      entries.forEach(en => {
        if (en.isIntersecting) {
          en.target.style.animationPlayState = 'running';
          io.unobserve(en.target);
        }
      });
    }, { threshold: 0.08 });
    document.querySelectorAll('.question-card').forEach(card => {
      card.style.animationPlayState = 'paused';
      io.observe(card);
    });
  }

  /* ==== Simple confetti particle system for results page ==== */
  function makeConfettiCanvas() {
    const existing = document.getElementById('confetti-canvas');
    if (existing) return existing;
    const c = document.createElement('canvas');
    c.id = 'confetti-canvas';
    document.body.appendChild(c);
    resizeCanvas(c);
    window.addEventListener('resize', ()=> resizeCanvas(c));
    return c;
  }
  function resizeCanvas(c) {
    c.width = window.innerWidth;
    c.height = window.innerHeight;
  }

  function fireConfetti(pieces = 120) {
    const canvas = makeConfettiCanvas();
    const ctx = canvas.getContext('2d');
    const W = canvas.width, H = canvas.height;
    const colors = ['#06b6d4','#7c3aed','#f97316','#06b981','#f43f5e','#fb7185'];
    const particles = [];
    for (let i=0;i<pieces;i++){
      particles.push({
        x: W/2 + (Math.random()-0.5)*200,
        y: H/3 + (Math.random()-0.5)*80,
        vx: (Math.random()-0.5)*8,
        vy: Math.random()*-6 - 2,
        r: Math.random()*8+4,
        color: colors[Math.floor(Math.random()*colors.length)],
        rot: Math.random()*360,
        vr: (Math.random()-0.5)*12,
        life: Math.random()*80+60
      });
    }
    let frame=0;
    function draw(){
      ctx.clearRect(0,0,W,H);
      frame++;
      for (let i=particles.length-1;i>=0;i--){
        const p = particles[i];
        p.vy += 0.35; // gravity
        p.x += p.vx;
        p.y += p.vy;
        p.rot += p.vr;
        p.life--;
        ctx.save();
        ctx.translate(p.x,p.y);
        ctx.rotate(p.rot*Math.PI/180);
        ctx.fillStyle = p.color;
        ctx.fillRect(-p.r/2, -p.r/2, p.r, p.r*0.6);
        ctx.restore();
        if (p.y > H + 50 || p.life <= 0) particles.splice(i,1);
      }
      if (particles.length) requestAnimationFrame(draw);
      else ctx.clearRect(0,0,W,H);
    }
    draw();
    // auto-remove canvas after a bit
    setTimeout(()=> {
      // fade out
      canvas.style.transition = 'opacity .8s';
      canvas.style.opacity = '0';
      setTimeout(()=> canvas.remove(), 900);
    }, 4000);
  }

  /* ==== init on DOM ready ==== */
  document.addEventListener('DOMContentLoaded', ()=> {
    createBlobs(); // decorative responsive blobs
    wireOptions();
    wireFormValidation();
    revealOnScroll();
    updateProgress();

    // If result page (detect result element), fire confetti
    const resultEl = document.querySelector('.result');
    if (resultEl) {
      setTimeout(()=> {
        fireConfetti(140);
      }, 300);
    }
  });

  /* Expose updateProgress for callers if needed */
  window.quizHelpers = { updateProgress, fireConfetti };

})();
