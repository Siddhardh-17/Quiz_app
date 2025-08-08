// script.js - small UI helpers for the quiz
document.addEventListener('DOMContentLoaded', () => {
  // add click handlers to option labels to toggle 'selected' class
  document.querySelectorAll('.option').forEach(label => {
    label.addEventListener('click', (e) => {
      const form = label.closest('form');
      // within this question, remove selected from siblings
      const qCard = label.closest('.question-card');
      if (!qCard) return;
      qCard.querySelectorAll('.option').forEach(el => el.classList.remove('selected'));
      label.classList.add('selected');
      // ensure the input is checked
      const input = label.querySelector('input[type="radio"]');
      if (input) input.checked = true;
    });
  });

  // form validation: require all questions answered
  const quizForm = document.getElementById('quiz-form');
  if (quizForm) {
    quizForm.addEventListener('submit', function(e){
      const qCards = document.querySelectorAll('.question-card');
      let answered = 0;
      qCards.forEach((qc, idx) => {
        const name = `q${idx}`;
        if (quizForm.querySelector(`input[name="${name}"]:checked`)) answered++;
      });
      if (answered < qCards.length) {
        e.preventDefault();
        alert(`Please answer all ${qCards.length} questions before submitting.`);
      }
    });
  }

});
