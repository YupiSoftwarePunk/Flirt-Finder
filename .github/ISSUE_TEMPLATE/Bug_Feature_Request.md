---
name: "Bug Report or Feature Request"
description: "Choose between reporting a bug or suggesting a new feature."
title: "[Bug/Feature]: "
labels: ["triage"]
assignees: ''
body:
  - type: markdown
    attributes:
      value: |
        Thanks for taking the time to help us improve!
        *For support, please use other channels like Discussions or Discord.*

  - type: dropdown
    id: type
    attributes:
      label: Issue Type
      description: What type of issue are you reporting?
      options:
        - Bug Report
        - Feature Request
    validations:
      required: true

  # --- BUG REPORT FIELDS ---
  - type: textarea
    id: bug_description
    attributes:
      label: "🐞 Bug Description"
      description: A clear and concise description of what the bug is.
      placeholder: "Tell us what's wrong..."
    validations:
      required: false # Only required if type is Bug Report

  - type: textarea
    id: steps
    attributes:
      label: "Steps to Reproduce"
      description: Detailed steps to reproduce the problematic behavior.
      value: |
        1. Go to '...'
        2. Click on '....'
        3. Scroll down to '....'
        4. See error
    validations:
      required: false

  - type: textarea
    id: expected_behavior
    attributes:
      label: "Expected Behavior"
      description: What did you expect to happen?
      placeholder: "It should have..."
    validations:
      required: false

  - type: textarea
    id: screenshots
    attributes:
      label: "Screenshots/Logs"
      description: If applicable, add screenshots, videos, or console logs to help explain your problem.
      placeholder: "You can drag and drop images here."
    validations:
      required: false

  # --- FEATURE REQUEST FIELDS ---
  - type: textarea
    id: problem
    attributes:
      label: "💡 Problem & Solution"
      description: Is your feature request related to a problem? Please describe and suggest a solution.
      placeholder: |
        **The Problem:**
        I'm always frustrated when [...]

        **The Solution:**
        I would like it to [...]
    validations:
      required: false # Only required if type is Feature Request

  - type: textarea
    id: alternatives
    attributes:
      label: "Alternatives Considered"
      description: A clear and concise description of any alternative solutions or features you've considered.
    validations:
      required: false

  # --- COMMON FIELDS ---
  - type: textarea
    id: additional_context
    attributes:
      label: "Additional Context"
      description: Add any other context about the problem or feature request here.
    validations:
      required: false

  - type: dropdown
    id: os
    attributes:
      label: "Operating System"
      description: What OS are you using?
      multiple: true
      options:
        - Windows
        - macOS
        - Linux (Please specify in Additional Context)
        - Other (Please specify in Additional Context)
    validations:
      required: false

  - type: input
    id: browser_version
    attributes:
      label: "Browser & Version"
      description: e.g., Chrome 122, Safari 17.4, Firefox 115
      placeholder: "Chrome 122"
    validations:
      required: false
