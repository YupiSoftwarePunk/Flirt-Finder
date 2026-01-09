using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Server.DTOs;
using Server.Services.Interfaces;
using System.Security.Claims;

namespace Server.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class MessagesController : ControllerBase
    {
        private readonly IMessageService _messageService;
        private readonly IChatService _chatService;
        private readonly IReportService _reportService;

        public MessagesController(
            IMessageService messageService,
            IChatService chatService,
            IReportService reportService)
        {
            _messageService = messageService;
            _chatService = chatService;
            _reportService = reportService;
        }


        // Отправка нового сообщения
        [HttpPost]
        public async Task<IActionResult> SendMessage([FromBody] MessageDto dto)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var senderId)) return Unauthorized();

            var message = await _messageService.SendAsync(senderId, dto);

            var response = new MessageDto
            {
                Id = message.Id,
                SenderId = message.SenderId,
                ReceiverId = message.ReceiverId,
                Content = message.Content,
                Timestamp = message.Timestamp,
                IsForwarded = message.IsForwarded
            };

            return Ok(response);
        }



        // Получение истории сообщений
        [HttpGet]
        public async Task<IActionResult> GetMessages([FromQuery] int userId)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var currentUserId)) return Unauthorized();

            var messages = await _messageService.GetChatHistoryAsync(currentUserId, userId);
            return Ok(messages);
        }



        // Удаление сообщения
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteMessage(int id)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            var success = await _messageService.DeleteAsync(userId, id);
            return success ? NoContent() : Forbid();
        }



        // Редактирование сообщения
        [HttpPatch("{id}")]
        public async Task<IActionResult> EditMessage(int id, [FromBody] EditMessageDto dto)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            var success = await _messageService.EditAsync(userId, id, dto);
            return success ? Ok() : BadRequest("Не удалось изменить сообщение");
        }



        // Получение одного сообщения (копирование)
        [HttpGet("{id}")]
        public async Task<IActionResult> GetMessage(int id)
        {
            var message = await _messageService.GetByIdAsync(id);
            return message != null ? Ok(message) : NotFound();
        }



        // Закрепить сообщение
        [HttpPost("/api/chats/{chatId}/pinned-messages")]
        public async Task<IActionResult> PinMessage(int chatId, [FromBody] PinMessageDto dto)
        {
            var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            var success = await _chatService.PinMessageAsync(userId, chatId, dto.MessageId);
            return success ? Ok() : BadRequest("Не удалось закрепить сообщение");
        }



        // Пожаловаться на сообщение
        [HttpPost("/api/reports")]
        public async Task<IActionResult> ReportMessage([FromBody] ReportMessageDto dto)
        {
            var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            await _reportService.ReportMessageAsync(userId, dto);
            return Ok("Жалоба отправлена");
        }



        [Authorize]
        [HttpPost("forward")]
        public async Task<IActionResult> ForwardMessage([FromBody] ForwardMessageDto dto)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var senderId)) return Unauthorized();

            await _messageService.ForwardAsync(senderId, dto);
            return Ok(new { message = "Сообщение переслано" });
        }
    }
}
