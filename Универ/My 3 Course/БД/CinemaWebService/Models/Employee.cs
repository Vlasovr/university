using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace CinemaWebService.Models
{
    public class Employee
    {
        [BsonId] // Указывает, что это поле является идентификатором документа
        [BsonRepresentation(BsonType.ObjectId)] // Позволяет использовать строковое представление ObjectId
        public string? Id { get; set; } // Свойство для хранения _id из MongoDB опционально для него, для posgres оно должно неопциональным
        public int EmployeeId { get; set; } // PK
        public string Phone { get; set; }
        public string Position { get; set; }
        public string FullName { get; set; }
        public string Email { get; set; }
        public string Address { get; set; }
        [BsonIgnore]
        public ICollection<SessionEmployee> SessionEmployees { get; set; } = new HashSet<SessionEmployee>();
    }
}