# Minishell Git Workflow

## 🌳 Overview
This document defines the **Git workflow** for the Minishell project. It ensures clear collaboration, safe merging, and stable project progress for a two-person team.

---

## 🌱 Branch Structure
```
main          ← Stable version (always compilable and tested)
│
└── dev       ← Development mainline (integrates all features)
     ├── feature/parser
     ├── feature/executor
     ├── feature/builtins
     ├── feature/signals
     └── feature/debug
```

### Branch Purpose
| Branch | Description | Who Can Modify |
|--------|--------------|----------------|
| **main** | Stable, tested version for final submission. | Both (with agreement) |
| **dev** | Main development branch. Features merge here for testing. | Both |
| **feature/*** | Each feature or module has its own branch. | Assigned developer |

---

## ⚙️ Setup & Initialization

### Initial Setup (done once by one teammate)
```bash
git init minishell
cd minishell
git checkout -b main
git add .
git commit -m "Initial commit: project setup"
git remote add origin git@github.com:<username>/minishell.git
git push -u origin main
git checkout -b dev
git push -u origin dev
```

---

## 🛠️ Daily Workflow

### 1. Start a Feature Branch
Each teammate creates their own feature branch from `dev`:

```bash
git checkout dev
git pull origin dev
git checkout -b feature/parser   # example for parser branch
```

### 2. Work and Commit Regularly
```bash
git add src/parser.c include/parser.h
git commit -m "Add basic tokenization and quote handling"
git push -u origin feature/parser
```

> Always commit small, logical chunks of work with clear messages.

### 3. Update Your Branch Frequently
Before pushing or merging:
```bash
git pull origin dev
# or
git fetch origin
git rebase origin/dev
```

---

## ⚒️ Merging Process

### 1. Merge Feature → Dev
When your feature is ready:
```bash
git checkout dev
git pull origin dev
git merge feature/parser
```
Resolve any conflicts, then:
```bash
git add .
git commit -m "Merge feature/parser into dev"
git push origin dev
```

### 2. Merge Dev → Main
When the project passes testing:
```bash
git checkout main
git pull origin main
git merge dev
git push origin main
```
Optionally, tag the version:
```bash
git tag v1.0
git push origin v1.0
```

---

## 📂 Recommended Folder Structure
```
minishell/
├── src/
│   ├── parser/
│   ├── executor/
│   ├── builtins/
│   ├── signals/
│   └── main.c
├── include/
│   ├── minishell.h
│   ├── parser.h
│   └── executor.h
├── libft/
├── Makefile
└── README.md
```

---

## 💬 Collaboration Tips

### Daily Routine
- Before starting work:
  ```bash
  git checkout dev
  git pull origin dev
  ```
- After finishing your session:
  ```bash
  git add .
  git commit -m "Describe what changed"
  git push origin feature/<branch>
  ```

### Every 2 days:
- Merge `feature/*` → `dev`
- Test combined build
- Fix merge conflicts together

### After full testing:
- Merge `dev` → `main`
- Tag release

---

## 🔗 Useful Commands
| Purpose | Command |
|----------|----------|
| View branches | `git branch -a` |
| Switch branch | `git checkout branch_name` |
| Pull latest | `git pull origin branch_name` |
| Merge branches | `git merge branch_name` |
| View history | `git log --oneline --graph --all` |
| Resolve conflict and continue | `git add . && git commit` |
| Delete local branch | `git branch -d feature/parser` |
| Delete remote branch | `git push origin --delete feature/parser` |

---

## 🛡️ Best Practices
- Protect `main` branch (use GitHub Branch Protection Rules).
- Run **Norminette** and **Valgrind** before merging.
- Use descriptive commits:
  ```
  feat(parser): add token split by quotes
  fix(executor): correct pipe fd closing
  refactor(env): improve export logic
  ```

---

**End of Minishell Git Workflow**

